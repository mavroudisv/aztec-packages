#include "barretenberg/vm2/tracegen/trace_container.hpp"

namespace bb::avm::tracegen {

void TraceContainer::set(Column col, size_t row, const FF& value)
{
    if (value != 0) {
        trace[col][row] = value;
    } else {
        trace[col].erase(row);
    }
}

void TraceContainer::set(size_t row, std::span<const std::pair<Column, FF>> values)
{
    for (const auto& [col, value] : values) {
        set(col, row, value);
    }
}

// TODO: we could cache this, but we'd have to watch out when we set a value to 0.
size_t TraceContainer::get_column_size(Column col) const
{
    auto keys = std::views::keys(trace.at(col));
    const auto it = std::max_element(keys.begin(), keys.end());
    return it == keys.end() ? 0 : *it + 1;
}

} // namespace bb::avm::tracegen