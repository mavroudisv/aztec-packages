#pragma once

#include <unordered_map>

#include "barretenberg/vm2/simulation/events/event_emitter.hpp"
#include "barretenberg/vm2/simulation/events/memory_event.hpp"

namespace bb::avm::simulation {

class Memory final {
  public:
    Memory(EventEmitterInterface<MemoryEvent>& event_emitter)
        : events(event_emitter)
    {}

    void set(size_t index, int value);
    int get(size_t index) const;

  private:
    std::unordered_map<size_t, int> memory;
    EventEmitterInterface<MemoryEvent>& events;
};

} // namespace bb::avm::simulation