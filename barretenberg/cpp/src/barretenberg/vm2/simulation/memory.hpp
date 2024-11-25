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

// class MemoryProviderInterface {
//   public:
//     virtual ~MemoryProviderInterface() = default;
//     virtual std::unique_ptr<MemoryInterface> make(uint32_t space_id,
//                                                   EventEmitterInterface<MemoryEvent>& event_emitter) const = 0;
// };

// class MemoryProvider : public MemoryProviderInterface {
//   public:
//     std::unique_ptr<MemoryInterface> make(uint32_t space_id,
//                                           EventEmitterInterface<MemoryEvent>& event_emitter) const override
//     {
//         return std::make_unique<Memory>(space_id, event_emitter);
//     }
// };

} // namespace bb::avm::simulation