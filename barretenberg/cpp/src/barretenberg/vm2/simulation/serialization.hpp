#pragma once

#include "barretenberg/numeric/uint128/uint128.hpp"
#include "barretenberg/vm2/common/field.hpp"
#include "barretenberg/vm2/common/memory_types.hpp"
#include "barretenberg/vm2/common/opcodes.hpp"

#include <cstdint>
#include <variant>
#include <vector>

namespace bb::avm::simulation {

struct Instruction {
    using Operand = std::variant<MemoryTag, uint8_t, uint16_t, uint32_t, uint64_t, uint128_t, FF>;
    WireOpCode op_code;
    std::vector<Operand> operands;

    Instruction() = delete;
    Instruction(WireOpCode op_code, std::vector<Operand> operands)
        : op_code(op_code)
        , operands(std::move(operands)){};
};

/**
 * @brief Parsing of an instruction in the supplied bytecode at byte position pos. This
 *        checks that the WireOpCode value is in the defined range and extracts the operands
 *        for each WireOpCode based on the specification from OPCODE_WIRE_FORMAT.
 *
 * @param bytecode The bytecode to be parsed as a vector of bytes/uint8_t
 * @param pos Bytecode position
 * @throws runtime_error exception when the bytecode is invalid or pos is out-of-range
 * @return The instruction
 */
Instruction decode_instruction(std::span<const uint8_t> bytecode, size_t pos);

} // namespace bb::avm::simulation
