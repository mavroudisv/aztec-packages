#pragma once

#include <cstdint>

#include "barretenberg/vm2/simulation/addressing.hpp"
#include "barretenberg/vm2/simulation/alu.hpp"
#include "barretenberg/vm2/simulation/events/event_emitter.hpp"
#include "barretenberg/vm2/simulation/events/execution_event.hpp"
#include "barretenberg/vm2/simulation/memory.hpp"

namespace bb::avm::simulation {

class Execution final {
  public:
    // TODO: receive an AddressingProvider.
    Execution(Memory& mem, Alu& alu, EventEmitterInterface<ExecutionEvent>& event_emitter)
        : memory(mem)
        , alu(alu)
        , events(event_emitter)
    {}

    void add(uint32_t a_operand, uint32_t b_operand, uint32_t dst_operand, uint8_t indirect);
    void call(uint32_t addr_operand, uint8_t indirect);

  private:
    Memory& memory;
    Alu& alu;
    EventEmitterInterface<ExecutionEvent>& events;
};

} // namespace bb::avm::simulation