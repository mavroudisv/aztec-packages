#pragma once

#include <cstdint>
#include <vector>

#include "barretenberg/vm2/common/memory_types.hpp"
#include "barretenberg/vm2/common/opcodes.hpp"

namespace bb::avm::simulation {

// List of opcodes that can be executed.
// This is like WireOpCode but without the variants.
// Order doesn't really matter as long as it's in sync with the circuit.
enum class ExecutionOpCode {
    ADD,
    CALL,
    JUMPI,
};

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