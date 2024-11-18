#pragma once

#include <cstdint>
#include <vector>

namespace bb::avm::simulation {

struct AddressingEvent {
    uint16_t indirect;
    std::vector<uint32_t> operands;
    std::vector<uint32_t> resolved_operands;
};

} // namespace bb::avm::simulation