#pragma once

#include <cstdint>
#include <unordered_map>

#include "barretenberg/vm2/common/opcodes.hpp"

namespace bb::avm {

struct InstructionSpec {
    struct GasInfo {
        uint16_t base_l2;
        uint16_t base_da;
        uint16_t dyn_l2;
        uint16_t dyn_da;
    };

    uint8_t num_addresses;
    GasInfo gas_cost;
};

// This is "extern" because the definition is in a different file.
extern const std::unordered_map<ExecutionOpCode, InstructionSpec> INSTRUCTION_SPEC;

} // namespace bb::avm