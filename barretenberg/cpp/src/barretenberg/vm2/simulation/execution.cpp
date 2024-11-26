#include "barretenberg/vm2/simulation/execution.hpp"
#include "barretenberg/vm2/common/opcodes.hpp"
#include "barretenberg/vm2/simulation/addressing.hpp"
#include "barretenberg/vm2/simulation/events/execution_event.hpp"

#include <cstdint>

namespace bb::avm::simulation {

void Execution::add(MemoryAddress a_operand, MemoryAddress b_operand, MemoryAddress dst_operand, uint8_t indirect)
{
    auto& memory = current_context().get_memory();
    auto [a_addr, b_addr, dst_addr] = addressing.resolve<3>(indirect, { a_operand, b_operand, dst_operand }, memory);

    alu.add(a_addr, b_addr, dst_addr);

    // TODO: this is repeated and should pc be here?
    auto pc = current_context().get_pc();
    events.emit({ .pc = pc,
                  .opcode = ExecutionOpCode::ADD,
                  .indirect = indirect,
                  .operands = { a_operand, b_operand, dst_operand },
                  .resolved_operands = { a_addr, b_addr, dst_addr } });
}

void Execution::call(MemoryAddress addr_operand, uint8_t indirect)
{
    auto& memory = current_context().get_memory();
    auto [addr] = addressing.resolve<1>(indirect, { addr_operand }, memory);

    // TODO: Maybe this should be done in a call gadget?
    // I can't do much more than resolve with the current event.
    auto contract_address = memory.get(addr);
    enter_context(context_provider.make(contract_address.value, /*call_id=*/0));

    auto pc = current_context().get_pc();
    events.emit({ .pc = pc,
                  .opcode = ExecutionOpCode::CALL,
                  .indirect = indirect,
                  .operands = { addr_operand },
                  .resolved_operands = { addr } });
}

void Execution::jumpi(uint32_t loc, MemoryAddress cond, uint8_t indirect)
{
    auto& memory = current_context().get_memory();
    auto [cond_addr] = addressing.resolve<1>(indirect, { cond }, memory);

    // TODO: in gadget.
    auto resolved_cond = memory.get(cond_addr);
    if (resolved_cond.value != 0) {
        current_context().set_next_pc(loc);
    }

    auto pc = current_context().get_pc();
    events.emit({ .pc = pc,
                  .opcode = ExecutionOpCode::JUMPI,
                  .indirect = indirect,
                  .operands = { loc, cond },
                  .resolved_operands = { loc, cond_addr } });
}

void Execution::run()
{
    // TODO: This would only execute one enqueued call.
    while (!context_stack.empty()) {
        auto& context = current_context();
        auto pc = context.get_pc();

        ExecutionOpCode opcode = ExecutionOpCode::ADD;
        std::vector<uint8_t> operands = { 0, 1, 2, 3 };
        uint32_t instruction_size = 4;
        uint32_t next_pc = pc + instruction_size;

        // TODO: maybe have parsing turn the opcode to the canonical/execution opcode form.
        switch (opcode) {
        case ExecutionOpCode::ADD:
            add(operands[1], operands[2], operands[3], /*indirect=*/operands[0]);
            break;
        case ExecutionOpCode::CALL:
            call(operands[0], /*indirect=*/operands[1]);
            break;
        case ExecutionOpCode::JUMPI:
            jumpi(operands[0], operands[1], operands[2]);
            break;
        default:
            // TODO: should be caught by parsing.
            throw std::runtime_error("Unknown opcode");
        }

        context.set_pc(next_pc);
    }
}

} // namespace bb::avm::simulation