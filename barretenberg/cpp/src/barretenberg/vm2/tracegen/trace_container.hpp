#pragma once

#include <algorithm>
#include <cstddef>
#include <ranges>
#include <unordered_map>

#include "barretenberg/vm2/generated/columns.hpp"
#include "barretenberg/vm2/generated/flavor_settings.hpp"

namespace bb::avm::tracegen {

class TraceContainer {
  public:
    using FF = AvmFlavorSettings::FF;

    void set(Column col, size_t row, FF&& value)
    {
        if (value != 0) {
            trace[col][row] = std::move(value);
        } else {
            trace[col].erase(row);
        }
    }

    // TODO: we could cache this, but we'd have to watch out when we set a value to 0.
    size_t get_column_size(Column col) const
    {
        auto keys = std::views::keys(trace.at(col));
        const auto it = std::max_element(keys.begin(), keys.end());
        return it == keys.end() ? 0 : *it + 1;
    }

  protected:
    // We store the trace as a sparse matrix.
    std::unordered_map<Column, /*rows*/ std::unordered_map<size_t, FF>> trace;
};

} // namespace bb::avm::tracegen