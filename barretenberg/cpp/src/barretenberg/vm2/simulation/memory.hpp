#pragma once

#include <memory>
#include <unordered_map>

#include "barretenberg/vm2/common/memory_types.hpp"
#include "barretenberg/vm2/simulation/events/event_emitter.hpp"
#include "barretenberg/vm2/simulation/events/memory_event.hpp"

namespace bb::avm::simulation {

struct ValueAndTag {
    MemoryValue value;
    MemoryTag tag;
};

class MemoryInterface {
  public:
    virtual ~MemoryInterface() = default;
    virtual void set(MemoryAddress index, MemoryValue value, MemoryTag tag) = 0;
    virtual ValueAndTag get(MemoryAddress index) const = 0;

    static bool is_valid_address(ValueAndTag address)
    {
        // TODO: consider adding < 2^32 once we change memory value types.
        return address.tag == MemoryTag::U32;
    }
};

class Memory : public MemoryInterface {
  public:
    Memory(uint32_t space_id, EventEmitterInterface<MemoryEvent>& event_emitter)
        : space_id(space_id)
        , events(event_emitter)
    {}

    void set(MemoryAddress index, MemoryValue value, MemoryTag tag) override;
    ValueAndTag get(MemoryAddress index) const override;

  private:
    uint32_t space_id;
    std::unordered_map<size_t, ValueAndTag> memory;
    EventEmitterInterface<MemoryEvent>& events;
};

} // namespace bb::avm::simulation