#pragma once

#include <cstdint>

namespace bb::avm {

using MemoryAddress = std::size_t;
using MemoryValue = int;

enum class MemoryTag {
    U1,
    U8,
    U16,
    U32,
    U64,
    U128,
    FF,
};

} // namespace bb::avm