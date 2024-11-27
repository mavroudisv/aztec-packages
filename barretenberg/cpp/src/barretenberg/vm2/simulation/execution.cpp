#include "barretenberg/vm2/simulation/execution.hpp"

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
    // TODO: track interaction.
    alu.add(context, a_addr, b_addr, dst_addr);
}

void Execution::call(ContextInterface& context, MemoryAddress addr)
{
    auto& memory = context.get_memory();

    // TODO: Maybe this should be done in a call gadget?
    // I can't do much more than resolve with the current event.
    auto contract_address = memory.get(addr);
    enter_context(context_provider.make(contract_address.value, /*call_id=*/0));
}

void Execution::jumpi(ContextInterface& context, uint32_t loc, MemoryAddress cond_addr)
{
    auto& memory = context.get_memory();

    // TODO: in gadget.
    auto resolved_cond = memory.get(cond_addr);
    if (resolved_cond.value != 0) {
        current_context().set_next_pc(loc);
    }
}

void Execution::run()
{
    // TODO: This would only execute one enqueued call.
    while (!context_stack.empty()) {
        auto& context = current_context();
        auto& memory = current_context().get_memory();
        auto pc = context.get_pc();

        // FIXME: this is a placeholder.
        ExecutionOpCode opcode = ExecutionOpCode::ADD;
        uint8_t indirect = 0;
        std::vector<MemoryAddress> operands = { 0, 1, 2, 3 };
        uint32_t instruction_size = 4;
        size_t num_addresses_to_resolve = 3;
        context.set_next_pc(pc + instruction_size);

        // By this point we assume that the opcode is one of the valid execution opcodes.
        // TODO: catch failure.
        auto resolved_operands = addressing.resolve(indirect, operands, num_addresses_to_resolve, memory);
        // TODO: consider passing context.
        dispatch_opcode(opcode, resolved_operands);

        events.emit({ .pc = pc,
                      .opcode = opcode,
                      .indirect = indirect,
                      .operands = operands,
                      .resolved_operands = resolved_operands });

        context.set_pc(context.get_next_pc());
    }
}

void Execution::dispatch_opcode(ExecutionOpCode opcode, const std::vector<MemoryAddress>& resolved_operands)
{
    switch (opcode) {
    case ExecutionOpCode::ADD:
        call_with_operands(&Execution::add, resolved_operands);
        break;
    case ExecutionOpCode::CALL:
        call_with_operands(&Execution::call, resolved_operands);
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
                                          const std::vector<MemoryAddress>& resolved_operands)
{
    assert(resolved_operands.size() == sizeof...(Ts));
    auto operand_indices = std::make_index_sequence<sizeof...(Ts)>{};
    using types = std::tuple<Ts...>;
    [f, this, &resolved_operands]<std::size_t... Is>(std::index_sequence<Is...>) {
        (this->*f)(current_context(), static_cast<std::tuple_element_t<Is, types>>(resolved_operands[Is])...);
    }(operand_indices);
}

} // namespace bb::avm::simulation