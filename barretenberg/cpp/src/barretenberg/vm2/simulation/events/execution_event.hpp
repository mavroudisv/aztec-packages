#pragma once

#include <cstdint>
#include <ostream>
#include <vector>

#include "barretenberg/vm2/common/aztec_types.hpp"
#include "barretenberg/vm2/common/instruction_spec.hpp"
#include "barretenberg/vm2/common/memory_types.hpp"
#include "barretenberg/vm2/common/opcodes.hpp"
#include "barretenberg/vm2/simulation/lib/serialization.hpp"

namespace bb::avm::simulation {

struct ExecutionEvent {
    uint32_t pc;
    ContractClassId contract_class_id;
    Instruction wire_instruction;
    InstructionSpec instruction_spec; // TODO: consider just having a reference.
    ExecutionOpCode opcode;
    std::vector<Operand> resolved_operands;

    bool operator==(const ExecutionEvent& other) const = default;
};

} // namespace bb::avm::simulation