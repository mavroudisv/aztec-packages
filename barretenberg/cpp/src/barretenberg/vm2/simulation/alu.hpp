#pragma once

#include <cstdint>
#include <memory>

#include "barretenberg/vm2/simulation/events/alu_event.hpp"
#include "barretenberg/vm2/simulation/events/event_emitter.hpp"
#include "barretenberg/vm2/simulation/memory.hpp"

namespace bb::avm::simulation {

class Alu final {
  public:
    Alu(Memory& mem, EventEmitterInterface<AluEvent>& event_emitter)
        : memory(mem)
        , events(event_emitter)
    {}

    // Operands are expected to be direct.
    void add(uint32_t a_addr, uint32_t b_addr, uint32_t dst_addr);

  private:
    Memory& memory;
    EventEmitterInterface<AluEvent>& events;
};

} // namespace bb::avm::simulation