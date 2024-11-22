#include "barretenberg/vm2/tracegen/test_trace_container.hpp"

namespace bb::avm::tracegen {

TestTraceContainer::RowTraceContainer TestTraceContainer::as_rows() const
{
    // Find the maximum size of any column.
    const size_t max_rows = [this]() {
        size_t max_size = 0;
        for (const auto& col : std::ranges::views::keys(trace)) {
            max_size = std::max(max_size, get_column_size(col));
        }
        return max_size;
    }();

    RowTraceContainer full_row_trace(max_rows);
    // Write the values.
    for (const auto& [col, row_value] : trace) {
        for (const auto& [row, value] : row_value) {
            full_row_trace[row].get_column(static_cast<ColumnAndShifts>(col)) = value;
        }
    }

    // Write the shifted values.
    // TODO: optimize by only looping through "to be shifted" columns.
    for (const auto& [col, row_value] : trace) {
        if (auto shifted = shift_column(col); shifted.has_value()) {
            for (const auto& [row, value] : row_value) {
                if (row == 0) {
                    continue;
                }
                full_row_trace[row - 1].get_column(shifted.value()) = value;
            }
        }
    }

    return full_row_trace;
}

} // namespace bb::avm::tracegen