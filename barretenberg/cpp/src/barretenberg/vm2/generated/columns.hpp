#pragma once

#include <array>
#include <optional>

namespace bb::avm {

// The entities that will be used in the flavor.
// clang-format off
#define PRECOMPUTED_ENTITIES precomputed
#define WIRE_ENTITIES last, pc, clk, alu_sel_op_add, alu_a, alu_b, alu_c, alu_operation, alu_a_addr, alu_b_addr, alu_dst_addr
#define DERIVED_WITNESS_ENTITIES some_other_column_inv
#define TO_BE_SHIFTED_COLUMNS Column::pc
#define SHIFTED_COLUMNS ColumnAndShifts::pc_shifted
#define SHIFTED_ENTITIES pc_shifted
#define ALL_ENTITIES PRECOMPUTED_ENTITIES, WIRE_ENTITIES, DERIVED_WITNESS_ENTITIES, SHIFTED_ENTITIES
#define UNSHIFTED_ENTITIES PRECOMPUTED_ENTITIES, WIRE_ENTITIES, DERIVED_WITNESS_ENTITIES
// clang-format on

// All columns minus shifts.
enum class Column { UNSHIFTED_ENTITIES };

// C++ doesn't allow enum extension, so we'll have to cast.
enum class ColumnAndShifts {
    ALL_ENTITIES,
    // Sentinel.
    NUM_COLUMNS,
};

constexpr auto TO_BE_SHIFTED_COLUMNS_ARRAY = []() { return std::array{ TO_BE_SHIFTED_COLUMNS }; }();
constexpr auto SHIFTED_COLUMNS_ARRAY = []() { return std::array{ SHIFTED_COLUMNS }; }();
static_assert(TO_BE_SHIFTED_COLUMNS_ARRAY.size() == SHIFTED_COLUMNS_ARRAY.size());

std::optional<ColumnAndShifts> shift_column(Column c);

} // namespace bb::avm