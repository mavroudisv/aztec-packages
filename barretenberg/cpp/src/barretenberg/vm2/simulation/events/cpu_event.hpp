#pragma once

#include <cstdint>
#include <vector>

#include "barretenberg/vm2/simulation/opcodes.hpp"

namespace bb::avm::simulation {

struct CpuEvent {
    OpCode opcode;
    uint16_t indirect;
    std::vector<uint32_t> operands;
    std::vector<uint32_t> resolved_operands;
};

} // namespace bb::avm::simulation