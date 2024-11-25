#pragma once

#include <unordered_map>

#include "barretenberg/vm2/simulation/events/event_emitter.hpp"
#include "barretenberg/vm2/simulation/events/memory_event.hpp"

namespace bb::avm::simulation {

class MemoryInterface {
  public:
    virtual ~MemoryInterface() = default;
    virtual void set(size_t index, int value) = 0;
    virtual int get(size_t index) const = 0;
};

class Memory : public MemoryInterface {
  public:
    Memory(EventEmitterInterface<MemoryEvent>& event_emitter)
        : events(event_emitter)
    {}

    void set(size_t index, int value) override;
    int get(size_t index) const override;

  private:
    std::unordered_map<size_t, int> memory;
    EventEmitterInterface<MemoryEvent>& events;
};

} // namespace bb::avm::simulation