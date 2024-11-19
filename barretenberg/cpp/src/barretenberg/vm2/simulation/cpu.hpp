#pragma once

#include <cstdint>
#include <memory>

#include "barretenberg/vm2/simulation/addressing.hpp"
#include "barretenberg/vm2/simulation/alu.hpp"
#include "barretenberg/vm2/simulation/events/cpu_event.hpp"
#include "barretenberg/vm2/simulation/events/event_emitter.hpp"
#include "barretenberg/vm2/simulation/memory.hpp"
#include "barretenberg/vm2/simulation/opcodes.hpp"

namespace bb::avm::simulation {

class Cpu final {
  public:
    // TODO: receive an AddressingProvider.
    Cpu(Memory& mem, Alu& alu, EventEmitterInterface<CpuEvent>& event_emitter)
        : memory(mem)
        , alu(alu)
        , events(event_emitter)
    {}

    void add(uint32_t a_operand, uint32_t b_operand, uint32_t dst_operand, uint8_t indirect)
    {
        auto [a_addr, b_addr, dst_addr] =
            Addressing<3>(indirect).resolve({ a_operand, b_operand, dst_operand }, memory);

        alu.add(a_addr, b_addr, dst_addr);

        events.emit(CpuEvent{ .opcode = OpCode::ADD,
                              .indirect = indirect,
                              .operands = { a_operand, b_operand, dst_operand },
                              .resolved_operands = { a_addr, b_addr, dst_addr } });
    }

    void call(uint32_t addr_operand, uint8_t indirect)
    {
        auto [addr] = Addressing<1>(indirect).resolve({ addr_operand }, memory);
        // TODO: Implement call
        (void)addr;
    }

  private:
    Memory& memory;
    Alu& alu;
    EventEmitterInterface<CpuEvent>& events;
};

} // namespace bb::avm::simulation