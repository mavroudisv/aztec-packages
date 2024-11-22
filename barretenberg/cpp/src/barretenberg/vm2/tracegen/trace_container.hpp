#pragma once

#include <algorithm>
#include <cstddef>
#include <ranges>
#include <span>
#include <unordered_map>

#include "barretenberg/vm2/generated/columns.hpp"
#include "barretenberg/vm2/generated/flavor_settings.hpp"

namespace bb::avm::tracegen {

class TraceContainer {
  public:
    using FF = AvmFlavorSettings::FF;

    void set(Column col, size_t row, const FF& value);
    // Bulk setting for a given row.
    void set(size_t row, std::span<const std::pair<Column, FF>> values);

    size_t get_column_size(Column col) const;

  protected:
    // We store the trace as a sparse matrix.
    std::unordered_map<Column, /*rows*/ std::unordered_map<size_t, FF>> trace;
};

} // namespace bb::avm::tracegen