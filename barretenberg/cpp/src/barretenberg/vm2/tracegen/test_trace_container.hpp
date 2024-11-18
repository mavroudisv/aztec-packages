#pragma once

#include <unordered_map>

#include "barretenberg/vm2/generated/full_row.hpp"
#include "barretenberg/vm2/tracegen/trace_container.hpp"

namespace bb::avm::tracegen {

class TestTraceContainer : public TraceContainer {
  public:
    using Row = AvmFullRow<FF>;

    std::vector<Row> as_rows() const
    {
        // Find the maximum size of any column.
        const size_t max_rows = [this]() {
            size_t max_size = 0;
            for (const auto& col : std::ranges::views::keys(trace)) {
                max_size = std::max(max_size, get_column_size(col));
            }
            return max_size;
        }();

        // Write the values.
        std::vector<Row> full_row_trace(max_rows);
        for (const auto& [col, row_value] : trace) {
            for (const auto& [row, value] : row_value) {
                full_row_trace[row].get_column(col) = value;
            }
        }

        return full_row_trace;
    }
};

} // namespace bb::avm::tracegen