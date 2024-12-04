#pragma once

#include <cstdint>
#include <vector>

#include "barretenberg/vm2/common/instruction_spec.hpp"
#include "barretenberg/vm2/common/memory_types.hpp"
#include "barretenberg/vm2/simulation/lib/serialization.hpp"

namespace bb::avm::simulation {

// See https://docs.google.com/document/d/1EgFj0OQYZCWufjzLgoAAiVL9jV0-fUAaCCIVlvRc8bY/ for circuit details.
// - The activation mask can be derived from spec.num_addresses.
struct AddressingEvent {
    Instruction instruction;
    std::vector<Operand> after_relative;
    std::vector<Operand> resolved_operands;
    MemoryValue stack_pointer_val;
    MemoryTag stack_pointer_tag;
    const InstructionSpec& spec;
};

} // namespace bb::avm::simulation