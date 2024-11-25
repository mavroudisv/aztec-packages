#pragma once

#include <memory>
#include <unordered_map>

#include "barretenberg/vm2/common/memory_types.hpp"
#include "barretenberg/vm2/simulation/events/event_emitter.hpp"
#include "barretenberg/vm2/simulation/events/memory_event.hpp"

namespace bb::avm::simulation {

class MemoryInterface {
  public:
    virtual ~MemoryInterface() = default;
    virtual void set(MemoryAddress index, MemoryValue value) = 0;
    virtual MemoryValue get(MemoryAddress index) const = 0;
};

class Memory : public MemoryInterface {
  public:
    Memory(uint32_t space_id, EventEmitterInterface<MemoryEvent>& event_emitter)
        : space_id(space_id)
        , events(event_emitter)
    {}

    void set(MemoryAddress index, MemoryValue value) override;
    MemoryValue get(MemoryAddress index) const override;

  private:
    uint32_t space_id;
    std::unordered_map<size_t, int> memory;
    EventEmitterInterface<MemoryEvent>& events;
};

} // namespace bb::avm::simulation