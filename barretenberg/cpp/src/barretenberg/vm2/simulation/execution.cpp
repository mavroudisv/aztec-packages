#include "barretenberg/vm2/simulation/execution.hpp"

#include <concepts>
#include <cstdint>
#include <functional>

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

void Execution::call(ContextInterface& context, MemoryAddress addr)
{
    auto& memory = context.get_memory();

    // TODO: Maybe this should be done in a call gadget?
    // I can't do much more than resolve with the current event.
    const auto [contract_address, _] = memory.get(addr);

    // TODO: should we do this in the main run() loop?
    // FIXME: I'm repeating everything that is in the run() loop and I don't like it.
    auto nested_context = context_provider.make(std::move(contract_address),
                                                /*msg_sender=*/context.get_address(),
                                                /*calldata=*/{},
                                                /*is_static=*/false);
    enter_context(std::move(nested_context));
}

void Execution::ret(ContextInterface& context, MemoryAddress ret_offset, MemoryAddress ret_size_offset)
{
    auto& memory = context.get_memory();

    // TODO: check tags and types (only for size, the return data is converted to FF).
    size_t size = static_cast<size_t>(memory.get(ret_size_offset).value);
    auto [values, _] = memory.get_slice(ret_offset, size);

    context_stack.pop();
    std::vector returndata(values.begin(), values.end());
    if (!context_stack.empty()) {
        auto& context = current_context();
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
    enter_context(std::move(context));
    run();
    return std::move(top_level_result);
}

void Execution::run()
{
    while (!context_stack.empty()) {
        auto& context = current_context();

        auto pc = context.get_pc();
        auto [instruction, read_bytes] = context.get_bytecode_manager().read_instruction(pc);
        auto [opcode, resolved_operands] = resolve_instruction(instruction);
        context.set_next_pc(pc + read_bytes);

        dispatch_opcode(opcode, resolved_operands);

        events.emit({
            .pc = pc,
            .opcode = opcode,
            .operands = {},         // operands,
            .resolved_operands = {} // resolved_operands
        });

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
        (this->*f)(current_context(), static_cast<std::tuple_element_t<Is, types>>(resolved_operands[Is])...);
    }(operand_indices);
}

template <typename... Ts>
constexpr size_t num_addresses_in_function([[maybe_unused]] void (Execution::*f)(ContextInterface&, Ts...))
{
    return ((std::is_same_v<Ts, MemoryAddress> ? 1 : 0) + ...);
}

std::pair<ExecutionOpCode, std::vector<Operand>> Execution::resolve_instruction(const Instruction& instruction)
{
    ExecutionOpCode opcode = static_cast<ExecutionOpCode>(-1);
    int64_t num_addresses_to_resolve = 0;

    switch (instruction.opcode) {
    case WireOpCode::ADD_8:
    case WireOpCode::ADD_16:
        opcode = ExecutionOpCode::ADD;
        num_addresses_to_resolve = num_addresses_in_function(&Execution::add);
        break;
    case WireOpCode::CALL:
        opcode = ExecutionOpCode::CALL;
        num_addresses_to_resolve = num_addresses_in_function(&Execution::call);
        break;
    case WireOpCode::RETURN:
        opcode = ExecutionOpCode::RETURN;
        num_addresses_to_resolve = num_addresses_in_function(&Execution::ret);
        break;
    case WireOpCode::JUMPI_32:
        opcode = ExecutionOpCode::JUMPI;
        num_addresses_to_resolve = num_addresses_in_function(&Execution::jumpi);
        break;
    default:
        (void)0;
        // throw std::runtime_error("Execution cannot handle opcode.");
    }

    // By this point we assume that the opcode is one of the valid execution opcodes.
    // TODO: catch failure.
    auto resolved_addresses =
        addressing.resolve(instruction.indirect,
                           std::vector<MemoryAddress>(instruction.operands.begin(),
                                                      instruction.operands.begin() + num_addresses_to_resolve),
                           current_context().get_memory());
    auto resolved_operands = instruction.operands;
    for (size_t i = 0; i < static_cast<size_t>(num_addresses_to_resolve); i++) {
        resolved_operands[i] = Operand(resolved_addresses[i]);
    }

    return { opcode, std::move(resolved_operands) };
}

} // namespace bb::avm::simulation