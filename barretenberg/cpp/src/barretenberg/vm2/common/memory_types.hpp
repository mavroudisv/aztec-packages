#pragma once

#include <cstdint>

#include "barretenberg/vm2/common/field.hpp"

namespace bb::avm {

using MemoryAddress = std::size_t;
using MemoryValue = simulation::FF;

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