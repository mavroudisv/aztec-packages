#pragma once

#include <array>

#include "barretenberg/vm2/simulation/events/addressing_event.hpp"
#include "barretenberg/vm2/simulation/events/event_emitter.hpp"
#include "barretenberg/vm2/simulation/memory.hpp"

namespace bb::avm::simulation {

// TODO: Make this class emit events.
template <size_t N> class Addressing final {
  public:
    Addressing(uint16_t indirect, EventEmitterInterface<AddressingEvent>& event_emitter)
        : indirect(indirect)
        , events(event_emitter)
    {}

    std::array<uint32_t, N> resolve(const std::array<uint32_t, N>& offsets, [[maybe_unused]] Memory& memory) const
    {
        // TODO: Doesn't do anything right now, but it could access memory.
        events.emit({ .indirect = indirect,
                      .operands = std::vector(offsets.begin(), offsets.end()),
                      .resolved_operands = std::vector(offsets.begin(), offsets.end()) });
        return offsets;
    }

  private:
    uint16_t indirect;
    EventEmitterInterface<AddressingEvent>& events;
};

class AddressingProvider final {
  public:
    AddressingProvider(EventEmitterInterface<AddressingEvent>& event_emitter)
        : events(event_emitter)
    {}
    template <size_t N> Addressing<N> get(uint16_t indirect) { return Addressing<N>(indirect, events); }

  private:
    EventEmitterInterface<AddressingEvent>& events;
};

} // namespace bb::avm::simulation