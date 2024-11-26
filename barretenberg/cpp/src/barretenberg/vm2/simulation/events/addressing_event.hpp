#pragma once

#include <cstdint>
#include <vector>

#include "barretenberg/vm2/common/memory_types.hpp"

namespace bb::avm::simulation {

struct AddressingEvent {
    uint16_t indirect;
    // FIXME: this should also accomodate immediates, etc.
    std::vector<MemoryAddress> operands;
    std::vector<MemoryAddress> resolved_operands;
};

} // namespace bb::avm::simulation