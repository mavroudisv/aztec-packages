#include "barretenberg/vm2/simulation/execution.hpp"

#include <algorithm>
#include <concepts>
#include <cstdint>
#include <functional>

#include "barretenberg/vm2/common/instruction_spec.hpp"
#include "barretenberg/vm2/common/memory_types.hpp"
#include "barretenberg/vm2/common/opcodes.hpp"
#include "barretenberg/vm2/simulation/addressing.hpp"
#include "barretenberg/vm2/simulation/context.hpp"
#include "barretenberg/vm2/simulation/events/execution_event.hpp"

namespace bb::avm::simulation {

void Execution::add(ContextInterface& context, MemoryAddress a_addr, MemoryAddress b_addr, MemoryAddress dst_addr)
{
    alu.add(context, a_addr, b_addr, dst_addr);
}

// TODO: This will need to happen in its own gadget in any case.
void Execution::call(ContextInterface& context, MemoryAddress addr)
{
    auto& memory = context.get_memory();

    const auto [contract_address, _] = memory.get(addr);
    std::vector<FF> calldata = {};

    auto nested_context = context_provider.make(std::move(contract_address),
                                                /*msg_sender=*/context.get_address(),
                                                /*calldata=*/calldata,
                                                /*is_static=*/false);
    context_stack.push(std::move(nested_context));
}

// TODO: This will need to happen in its own gadget in any case.
void Execution::ret(ContextInterface& context, MemoryAddress ret_offset, MemoryAddress ret_size_offset)
{
    auto& memory = context.get_memory();

    // TODO: check tags and types (only for size, the return data is converted to FF).
    size_t size = static_cast<size_t>(memory.get(ret_size_offset).value);
    auto [values, _] = memory.get_slice(ret_offset, size);

    context_stack.pop();
    std::vector returndata(values.begin(), values.end());
    if (!context_stack.empty()) {
        auto& context = context_stack.current();
        // TODO: We'll need more than just the return data. E.g., the space id, address and size.
        context.set_nested_returndata(std::move(returndata));
    } else {
        top_level_result = {
            .returndata = std::move(returndata),
            .success = true,
        };
    }
}

void Execution::jumpi(ContextInterface& context, uint32_t loc, MemoryAddress cond_addr)
{
    auto& memory = context.get_memory();

    // TODO: in gadget.
    auto resolved_cond = memory.get(cond_addr);
    if (resolved_cond.value != 0) {
        context.set_next_pc(loc);
    }
}

ExecutionResult Execution::execute(AztecAddress contract_address,
                                   std::span<const FF> calldata,
                                   AztecAddress msg_sender,
                                   bool is_static)
{
    auto context = context_provider.make(contract_address, msg_sender, calldata, is_static);
    context_stack.push(std::move(context));
    execution_loop();
    return std::move(top_level_result);
}

void Execution::execution_loop()
{
    while (!context_stack.empty()) {
        auto& context = context_stack.current();

        auto pc = context.get_pc();
        Instruction instruction = context.get_bytecode_manager().read_instruction(pc);
        ExecutionOpCode opcode = instruction_info_db.map_wire_opcode_to_execution_opcode(instruction.opcode);
        InstructionSpec spec = instruction_info_db.get(opcode);
        std::vector<Operand> resolved_operands = resolve_operands(instruction, spec);
        context.set_next_pc(pc + instruction.size_in_bytes);

        dispatch_opcode(opcode, resolved_operands);

        events.emit({ .pc = pc,
                      .contract_class_id = context.get_bytecode_manager().get_class_id(),
                      .wire_instruction = std::move(instruction),
                      .instruction_spec = std::move(spec),
                      .opcode = opcode,
                      .resolved_operands = resolved_operands });

        context.set_pc(context.get_next_pc());
    }
}

void Execution::dispatch_opcode(ExecutionOpCode opcode, const std::vector<Operand>& resolved_operands)
{
    switch (opcode) {
    case ExecutionOpCode::ADD:
        call_with_operands(&Execution::add, resolved_operands);
        break;
    case ExecutionOpCode::CALL:
        call_with_operands(&Execution::call, resolved_operands);
        break;
    case ExecutionOpCode::RETURN:
        call_with_operands(&Execution::ret, resolved_operands);
        break;
    case ExecutionOpCode::JUMPI:
        call_with_operands(&Execution::jumpi, resolved_operands);
        break;
    default:
        // TODO: should be caught by parsing.
        throw std::runtime_error("Unknown opcode");
    }
}

// Some template magic to dispatch the opcode by deducing the number of arguments and types,
// and making the appropriate checks and casts.
template <typename... Ts>
inline void Execution::call_with_operands(void (Execution::*f)(ContextInterface&, Ts...),
                                          const std::vector<Operand>& resolved_operands)
{
    assert(resolved_operands.size() == sizeof...(Ts));
    auto operand_indices = std::make_index_sequence<sizeof...(Ts)>{};
    using types = std::tuple<Ts...>;
    [f, this, &resolved_operands]<std::size_t... Is>(std::index_sequence<Is...>) {
        (this->*f)(context_stack.current(), static_cast<std::tuple_element_t<Is, types>>(resolved_operands[Is])...);
    }(operand_indices);
}

std::vector<Operand> Execution::resolve_operands(const Instruction& instruction, const InstructionSpec& spec)
{
    assert(instruction.operands.size() <= spec.num_addresses);
    // TODO: catch failure.
    std::vector<MemoryAddress> resolved_addresses = addressing.resolve(
        instruction.indirect,
        std::vector<MemoryAddress>(instruction.operands.begin(), instruction.operands.begin() + spec.num_addresses),
        context_stack.current().get_memory());
    auto resolved_operands = instruction.operands;
    for (size_t i = 0; i < static_cast<size_t>(spec.num_addresses); i++) {
        resolved_operands[i] = Operand(resolved_addresses[i]);
    }

    return resolved_operands;
}

} // namespace bb::avm::simulation