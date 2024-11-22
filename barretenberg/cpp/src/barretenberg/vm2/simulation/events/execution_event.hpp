#pragma once

#include <cstdint>
#include <vector>

#include "barretenberg/vm2/common/opcodes.hpp"

namespace bb::avm::simulation {

// List of opcodes that can be executed.
// This is like WireOpCode but without the variants.
// Order doesn't really matter as long as it's in sync with the circuit.
enum class ExecutionOpCode {
    ADD,
};

struct ExecutionEvent {
    ExecutionOpCode opcode;
    uint16_t indirect;
    std::vector<uint32_t> operands;
    std::vector<uint32_t> resolved_operands;
};

} // namespace bb::avm::simulation