#pragma once

#include <cstdint>
#include <ostream>
#include <vector>

#include "barretenberg/vm2/common/memory_types.hpp"
#include "barretenberg/vm2/common/opcodes.hpp"

namespace bb::avm::simulation {

struct ExecutionEvent {
    uint32_t pc;
    ExecutionOpCode opcode;
    uint16_t indirect;
    // FIXME: this should also accomodate immediates, etc.
    std::vector<MemoryAddress> operands;
    std::vector<MemoryAddress> resolved_operands;

    // Needed for testing matchers.
    bool operator==(const ExecutionEvent& other) const = default;
};

inline std::ostream& operator<<(std::ostream& os, const ExecutionEvent& s)
{
    os << "ExecutionEvent{pc=" << s.pc << ", opcode=" << s.opcode << ", indirect=" << s.indirect << ", operands=[";
    for (const auto& operand : s.operands) {
        os << operand << ", ";
    }
    os << "], resolved_operands=[";
    for (const auto& resolved_operand : s.resolved_operands) {
        os << resolved_operand << ", ";
    }
    os << "]}";
    return os;
}

} // namespace bb::avm::simulation