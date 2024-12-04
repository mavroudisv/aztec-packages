#pragma once

#include <cstdint>
#include <span>

#include "barretenberg/vm2/common/field.hpp"

namespace bb::avm::simulation {

FF compute_public_bytecode_commitment(std::span<const uint8_t> bytecode);
// TODO: add contract class id computation etc here.

} // namespace bb::avm::simulation