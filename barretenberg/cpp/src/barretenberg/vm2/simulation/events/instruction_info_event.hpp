#pragma once

#include "barretenberg/vm2/simulation/execution.hpp"

namespace bb::avm::simulation {

// This event signifies that we've accessed the instruction information for a given instruction.
struct InstructionInformationEvent {
    ExecutionOpCode opcode;
};

} // namespace bb::avm::simulation