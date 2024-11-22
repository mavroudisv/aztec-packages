#include "barretenberg/vm2/generated/columns.hpp"

#include <unordered_map>

namespace bb::avm {

std::optional<ColumnAndShifts> shift_column(Column c)
{
    static std::unordered_map<Column, ColumnAndShifts> shifts = []() {
        std::unordered_map<Column, ColumnAndShifts> shifts;
        for (size_t i = 0; i < TO_BE_SHIFTED_COLUMNS_ARRAY.size(); ++i) {
            shifts[TO_BE_SHIFTED_COLUMNS_ARRAY[i]] = SHIFTED_COLUMNS_ARRAY[i];
        }
        return shifts;
    }();

    auto it = shifts.find(c);
    return it == shifts.end() ? std::nullopt : std::make_optional(it->second);
}

} // namespace bb::avm