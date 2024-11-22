#include "barretenberg/vm2/simulation/execution.hpp"
#include "barretenberg/vm2/simulation/events/execution_event.hpp"

#include <cstdint>

namespace bb::avm::simulation {

void Execution::add(uint32_t a_operand, uint32_t b_operand, uint32_t dst_operand, uint8_t indirect)
{
    auto [a_addr, b_addr, dst_addr] = Addressing<3>(indirect).resolve({ a_operand, b_operand, dst_operand }, memory);

    alu.add(a_addr, b_addr, dst_addr);

    events.emit(ExecutionEvent{ .opcode = ExecutionOpCode::ADD,
                                .indirect = indirect,
                                .operands = { a_operand, b_operand, dst_operand },
                                .resolved_operands = { a_addr, b_addr, dst_addr } });
}

void Execution::call(uint32_t addr_operand, uint8_t indirect)
{
    auto [addr] = Addressing<1>(indirect).resolve({ addr_operand }, memory);
    // TODO: Implement call
    (void)addr;
}

} // namespace bb::avm::simulation