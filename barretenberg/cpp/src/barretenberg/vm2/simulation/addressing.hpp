#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <vector>

#include "barretenberg/vm2/common/memory_types.hpp"
#include "barretenberg/vm2/simulation/events/addressing_event.hpp"
#include "barretenberg/vm2/simulation/events/event_emitter.hpp"
#include "barretenberg/vm2/simulation/memory.hpp"

namespace bb::avm::simulation {

class AddressingBase {
  public:
    virtual ~AddressingBase() = default;
    // We need this method to be non-templated so that we can mock it.
    virtual std::vector<MemoryAddress> resolve(uint16_t indirect,
                                               const std::vector<MemoryAddress>& offsets,
                                               size_t num_addresses_to_resolve,
                                               MemoryInterface& memory) const = 0;

    // Convenience function that returns an array so that it can be destructured.
    // template <size_t N>
    // std::array<MemoryAddress, N> resolve(uint16_t indirect,
    //                                      const std::array<MemoryAddress, N>& offsets,
    //                                      MemoryInterface& memory) const
    // {
    //     assert(offsets.size() == N);
    //     auto resolved = resolve_(indirect, std::vector(offsets.begin(), offsets.end()), memory);
    //     std::array<MemoryAddress, N> result;
    //     std::copy(resolved.begin(), resolved.end(), result.begin());
    //     return result;
    // }
};

class Addressing final : public AddressingBase {
  public:
    Addressing(EventEmitterInterface<AddressingEvent>& event_emitter)
        : events(event_emitter)
    {}

    std::vector<MemoryAddress> resolve(uint16_t indirect,
                                       const std::vector<MemoryAddress>& offsets,
                                       size_t num_addresses_to_resolve,
                                       MemoryInterface& memory) const override;

  private:
    EventEmitterInterface<AddressingEvent>& events;
};

} // namespace bb::avm::simulation