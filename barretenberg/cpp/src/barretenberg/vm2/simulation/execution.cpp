#include "barretenberg/vm2/simulation/execution.hpp"
#include "barretenberg/vm2/simulation/addressing.hpp"
#include "barretenberg/vm2/simulation/events/execution_event.hpp"

#include <cstdint>

namespace bb::avm::simulation {

void Execution::add(MemoryAddress a_operand, MemoryAddress b_operand, MemoryAddress dst_operand, uint8_t indirect)
{
    auto& memory = current_context().get_memory();
    auto [a_addr, b_addr, dst_addr] = addressing.resolve<3>(indirect, { a_operand, b_operand, dst_operand }, memory);

    alu.add(a_addr, b_addr, dst_addr);

    events.emit({ .opcode = ExecutionOpCode::ADD,
                  .indirect = indirect,
                  .operands = { a_operand, b_operand, dst_operand },
                  .resolved_operands = { a_addr, b_addr, dst_addr } });
}

void Execution::call(MemoryAddress addr_operand, uint8_t indirect)
{
    auto& memory = current_context().get_memory();
    auto [addr] = addressing.resolve<1>(indirect, { addr_operand }, memory);

    // TODO: Implement call
    auto contract_address = memory.get(addr);
    (void)contract_address;
    // enter_context(context_provider.make());
}

} // namespace bb::avm::simulation