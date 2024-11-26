#include "barretenberg/vm2/simulation/addressing.hpp"

#include <cstdint>
#include <vector>

#include "barretenberg/vm2/common/memory_types.hpp"
#include "barretenberg/vm2/simulation/events/addressing_event.hpp"
#include "barretenberg/vm2/simulation/events/event_emitter.hpp"
#include "barretenberg/vm2/simulation/events/memory_event.hpp"
#include "barretenberg/vm2/simulation/memory.hpp"

namespace bb::avm::simulation {

// Will resolve `num_addresses_to_resolve` addresses from `offsets` using the memory interface.
// This assumes that the addresses are always the first operands.
std::vector<MemoryAddress> Addressing::resolve(uint16_t indirect,
                                               const std::vector<MemoryAddress>& offsets,
                                               size_t num_addresses_to_resolve,
                                               MemoryInterface& memory) const
{
    const size_t num_operands = offsets.size();
    assert(num_operands <= num_addresses_to_resolve);
    // Copy the original offsets since they are our starting point.
    std::vector<MemoryAddress> resolved(offsets.begin(), offsets.end());

    // We retrieve, cache and check M[0] first because this is probably what we'll do in the circuit.
    auto stack_pointer = memory.get(0);
    // TODO: propagate this error.
    assert(memory.is_valid_address(stack_pointer));

    for (size_t i = 0; i < num_addresses_to_resolve; ++i) {
        // Relative?
        if ((resolved[i] >> i) & 1) {
            // TODO: check bounds and tags. See simulator for reference.
            resolved[i] += static_cast<MemoryAddress>(stack_pointer.value);
        }
        // Indirect?
        if ((resolved[i] >> (i + num_addresses_to_resolve)) & 1) {
            // TODO: check bounds and tags. See simulator for reference.
            auto new_address = memory.get(resolved[i]);
            assert(memory.is_valid_address(new_address));
            resolved[i] = static_cast<MemoryAddress>(new_address.value);
        }
    }

    events.emit({ .num_addresses_to_resolve = num_addresses_to_resolve,
                  .indirect = indirect,
                  .operands = offsets,
                  .resolved_operands = resolved });
    return resolved;
}

} // namespace bb::avm::simulation