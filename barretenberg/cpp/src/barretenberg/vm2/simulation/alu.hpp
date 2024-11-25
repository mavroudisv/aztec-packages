#pragma once

#include <cstdint>
#include <memory>

#include "barretenberg/vm2/simulation/events/alu_event.hpp"
#include "barretenberg/vm2/simulation/events/event_emitter.hpp"
#include "barretenberg/vm2/simulation/memory.hpp"

namespace bb::avm::simulation {

class AluInterface {
  public:
    virtual ~AluInterface() = default;
    virtual void add(uint32_t a_addr, uint32_t b_addr, uint32_t dst_addr) = 0;
};

class Alu : public AluInterface {
  public:
    Alu(MemoryInterface& mem, EventEmitterInterface<AluEvent>& event_emitter)
        : memory(mem)
        , events(event_emitter)
    {}

    // Operands are expected to be direct.
    void add(uint32_t a_addr, uint32_t b_addr, uint32_t dst_addr) override;

  private:
    MemoryInterface& memory;
    EventEmitterInterface<AluEvent>& events;
};

} // namespace bb::avm::simulation