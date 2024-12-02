#pragma once

#include <cstdint>
#include <ostream>
#include <vector>

#include "barretenberg/vm2/common/aztec_types.hpp"
#include "barretenberg/vm2/common/memory_types.hpp"
#include "barretenberg/vm2/common/opcodes.hpp"
#include "barretenberg/vm2/simulation/lib/serialization.hpp"

namespace bb::avm::simulation {

struct ExecutionEvent {
    uint32_t pc;
    ExecutionOpCode opcode;
    ContractClassId contract_class_id;
    std::vector<Operand> operands;
    std::vector<Operand> resolved_operands;

    // Needed for testing matchers.
    bool operator==(const ExecutionEvent& other) const = default;
};

} // namespace bb::avm::simulation