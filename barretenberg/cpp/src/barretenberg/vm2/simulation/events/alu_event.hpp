#pragma once

#include <cstdint>
#include <vector>

#include "barretenberg/vm2/simulation/opcodes.hpp"

namespace bb::avm::simulation {

struct AluEvent {
    OpCode opcode;
    uint32_t a_addr;
    uint32_t b_addr;
    uint32_t dst_addr;
    int a;
    int b;
    int res;
};

} // namespace bb::avm::simulation