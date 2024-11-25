#include "barretenberg/vm2/simulation/addressing.hpp"

#include "barretenberg/vm2/common/memory_types.hpp"
#include "barretenberg/vm2/simulation/events/addressing_event.hpp"
#include "barretenberg/vm2/simulation/events/event_emitter.hpp"
#include "barretenberg/vm2/simulation/memory.hpp"
#include <cstdint>

namespace bb::avm::simulation {

std::vector<MemoryAddress> Addressing::resolve_(uint16_t indirect,
                                                const std::vector<MemoryAddress>& offsets,
                                                MemoryInterface& memory) const
{
    const size_t num_operands = offsets.size();
    std::vector<MemoryAddress> resolved(num_operands);

    for (size_t i = 0; i < offsets.size(); ++i) {
        resolved[i] = offsets[i];
        // Relative?
        if ((resolved[i] >> i) & 1) {
            // TODO: check bounds and tags. See simulator for reference.
            resolved[i] += static_cast<uint32_t>(memory.get(0));
        }
        // Indirect?
        if ((resolved[i] >> (i + num_operands)) & 1) {
            // TODO: check bounds and tags. See simulator for reference.
            resolved[i] = static_cast<uint32_t>(memory.get(resolved[i]));
        }
    }

    events.emit({ .indirect = indirect, .operands = offsets, .resolved_operands = resolved });
    return resolved;
}

} // namespace bb::avm::simulation