#pragma once

#include <cstdint>
#include <memory>

#include "barretenberg/vm2/common/memory_types.hpp"
#include "barretenberg/vm2/simulation/events/alu_event.hpp"
#include "barretenberg/vm2/simulation/events/event_emitter.hpp"
#include "barretenberg/vm2/simulation/memory.hpp"

namespace bb::avm::simulation {

class AluInterface {
  public:
    virtual ~AluInterface() = default;
    virtual void add(MemoryAddress a_addr, MemoryAddress b_addr, MemoryAddress dst_addr) = 0;
};

class Alu : public AluInterface {
  public:
    Alu(MemoryInterface& mem, EventEmitterInterface<AluEvent>& event_emitter)
        : memory(mem)
        , events(event_emitter)
    {}

    // Operands are expected to be direct.
    void add(MemoryAddress a_addr, MemoryAddress b_addr, MemoryAddress dst_addr) override;

  private:
    MemoryInterface& memory;
    EventEmitterInterface<AluEvent>& events;
};

} // namespace bb::avm::simulation