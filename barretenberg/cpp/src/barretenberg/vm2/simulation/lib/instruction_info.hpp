#pragma once

#include <cstdint>
#include <memory>

#include "barretenberg/vm2/common/instruction_spec.hpp"
#include "barretenberg/vm2/common/opcodes.hpp"

namespace bb::avm::simulation {

class InstructionInfoDBInterface {
  public:
    virtual ~InstructionInfoDBInterface() = default;

    virtual const InstructionSpec& get(ExecutionOpCode opcode) const = 0;
    virtual ExecutionOpCode map_wire_opcode_to_execution_opcode(WireOpCode opcode) const = 0;
};

class InstructionInfoDB : public InstructionInfoDBInterface {
  public:
    const InstructionSpec& get(ExecutionOpCode opcode) const override { return INSTRUCTION_SPEC.at(opcode); }
    ExecutionOpCode map_wire_opcode_to_execution_opcode(WireOpCode opcode) const override
    {
        return OPCODE_MAP.at(opcode);
    }
};

} // namespace bb::avm::simulation