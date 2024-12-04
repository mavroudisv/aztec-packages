#pragma once

#include <cstdint>
#include <memory>

#include <gmock/gmock.h>

#include "barretenberg/vm2/simulation/lib/instruction_info.hpp"

namespace bb::avm::simulation {

class MockInstructionInfoDB : public InstructionInfoDBInterface {
  public:
    // TODO: do later.
    // https://google.github.io/googletest/gmock_cook_book.html#making-the-compilation-faster
    // MockInstructionInfo();
    // ~MockInstructionInfo() override;

    MOCK_METHOD(InstructionSpec, get, (ExecutionOpCode opcode), (const override));
    MOCK_METHOD(ExecutionOpCode, map_wire_opcode_to_execution_opcode, (WireOpCode opcode), (const override));
};

} // namespace bb::avm::simulation