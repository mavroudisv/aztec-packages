#pragma once

#include <cstdint>
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
    const InstructionSpec& instruction_spec;
    ExecutionOpCode opcode;
    std::vector<Operand> resolved_operands;

    // We need to write this manually only because there struct contains a reference.
    bool operator==(const ExecutionEvent& other) const
    {
        return pc == other.pc && contract_class_id == other.contract_class_id &&
               wire_instruction == other.wire_instruction && instruction_spec == other.instruction_spec &&
               opcode == other.opcode && resolved_operands == other.resolved_operands;
    }
};

} // namespace bb::avm::simulation