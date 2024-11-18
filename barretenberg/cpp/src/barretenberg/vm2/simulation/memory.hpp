#pragma once

#include <memory>
#include <unordered_map>

#include "barretenberg/vm2/simulation/events/event_emitter.hpp"
#include "barretenberg/vm2/simulation/events/memory_event.hpp"

namespace bb::avm::simulation {

class Memory final {
  public:
    Memory(EventEmitterInterface<MemoryEvent>& event_emitter)
        : events(event_emitter)
    {}

    void set(size_t index, int value)
    {
        memory[index] = value;
        events.emit(MemoryEvent{ .mode = MemoryMode::WRITE, .addr = index, .value = value });
    }
    int get(size_t index) const
    {
        auto value = memory.at(index);
        events.emit(MemoryEvent{ .mode = MemoryMode::READ, .addr = index, .value = value });
        return value;
    }

  private:
    std::unordered_map<size_t, int> memory;
    EventEmitterInterface<MemoryEvent>& events;
};

} // namespace bb::avm::simulation