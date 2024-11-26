#pragma once

#include <cstdint>
#include <vector>

#include "barretenberg/vm2/common/memory_types.hpp"
#include "barretenberg/vm2/common/opcodes.hpp"

namespace bb::avm::simulation {

struct ExecutionEvent {
    uint32_t pc;
    ExecutionOpCode opcode;
    uint16_t indirect;
    // FIXME: this should also accomodate immediates, etc.
    std::vector<MemoryAddress> operands;
    std::vector<MemoryAddress> resolved_operands;

    // Needed for testing matchers.
    bool operator==(const ExecutionEvent& other) const = default;
};

} // namespace bb::avm::simulation