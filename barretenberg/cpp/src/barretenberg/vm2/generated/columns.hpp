#pragma once

namespace bb::avm {

// The entities that will be used in the flavor.
// clang-format off
#define PRECOMPUTED_ENTITIES precomputed
#define WIRE_ENTITIES alu_a, alu_b, alu_c, alu_opcode, alu_a_addr, alu_b_addr, alu_dst_addr
#define DERIVED_WITNESS_ENTITIES some_other_column_inv
#define SHIFTED_ENTITIES
#define TO_BE_SHIFTED(e)
#define ALL_ENTITIES PRECOMPUTED_ENTITIES, WIRE_ENTITIES, DERIVED_WITNESS_ENTITIES, SHIFTED_ENTITIES
#define UNSHIFTED_ENTITIES PRECOMPUTED_ENTITIES, WIRE_ENTITIES, DERIVED_WITNESS_ENTITIES
// clang-format on

// All columns minus shifts.
enum class Column {
    UNSHIFTED_ENTITIES,
    // Sentinel.
    NUM_COLUMNS,
};

} // namespace bb::avm