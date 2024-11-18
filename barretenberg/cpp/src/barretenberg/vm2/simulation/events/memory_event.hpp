#pragma once

#include <cstdint>
#include <vector>

namespace bb::avm::simulation {

enum class MemoryMode {
    READ,
    WRITE,
};

struct MemoryEvent {
    MemoryMode mode;
    size_t addr;
    int value;
};

} // namespace bb::avm::simulation