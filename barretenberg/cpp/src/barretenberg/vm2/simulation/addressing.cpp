#include "barretenberg/vm2/simulation/addressing.hpp"

#include "barretenberg/vm2/common/memory_types.hpp"
#include "barretenberg/vm2/simulation/events/addressing_event.hpp"
#include "barretenberg/vm2/simulation/events/event_emitter.hpp"
#include "barretenberg/vm2/simulation/events/memory_event.hpp"
#include "barretenberg/vm2/simulation/memory.hpp"
#include <cstdint>

namespace bb::avm::simulation {

std::vector<MemoryAddress> Addressing::resolve_(uint16_t indirect,
                                                const std::vector<MemoryAddress>& offsets,
                                                MemoryInterface& memory) const
{
    const size_t num_operands = offsets.size();
    std::vector<MemoryAddress> resolved(num_operands);

    // We retrieve, cache and check M[0] first because this is probably what we'll do in the circuit.
    auto stack_pointer = memory.get(0);
    // TODO: propagate this error.
    assert(memory.is_valid_address(stack_pointer));

    for (size_t i = 0; i < offsets.size(); ++i) {
        resolved[i] = offsets[i];
        // Relative?
        if ((resolved[i] >> i) & 1) {
            // TODO: check bounds and tags. See simulator for reference.
            resolved[i] += static_cast<MemoryAddress>(stack_pointer.value);
        }
        // Indirect?
        if ((resolved[i] >> (i + num_operands)) & 1) {
            // TODO: check bounds and tags. See simulator for reference.
            auto new_address = memory.get(resolved[i]);
            assert(memory.is_valid_address(new_address));
            resolved[i] = static_cast<MemoryAddress>(new_address.value);
        }
    }

    events.emit({ .indirect = indirect, .operands = offsets, .resolved_operands = resolved });
    return resolved;
}

} // namespace bb::avm::simulation