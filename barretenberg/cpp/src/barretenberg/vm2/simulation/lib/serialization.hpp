#pragma once

#include "barretenberg/numeric/uint128/uint128.hpp"
#include "barretenberg/vm2/common/field.hpp"
#include "barretenberg/vm2/common/memory_types.hpp"
#include "barretenberg/vm2/common/opcodes.hpp"

#include <cstdint>
#include <variant>
#include <vector>

namespace bb::avm::simulation {

class Operand {
  public:
    Operand(std::variant<uint8_t, uint16_t, uint32_t, uint64_t, uint128_t, FF> value)
        : value(std::move(value))
    {}

    // Helpers for when we want to pass a value without casting.
    static Operand u8(uint8_t value) { return { value }; }
    static Operand u16(uint16_t value) { return { value }; }
    static Operand u32(uint32_t value) { return { value }; }
    static Operand u64(uint64_t value) { return { value }; }
    static Operand u128(uint128_t value) { return { std::move(value) }; }
    static Operand ff(FF value) { return { std::move(value) }; }

    // We define conversion to supported types.
    // The conversion will throw if the type would truncate.
    explicit operator bool() const;
    explicit operator uint8_t() const;
    explicit operator uint16_t() const;
    explicit operator uint32_t() const;
    explicit operator uint64_t() const;
    explicit operator uint128_t() const;
    explicit operator FF() const;

    bool operator==(const Operand& other) const = default;

  private:
    std::variant<uint8_t, uint16_t, uint32_t, uint64_t, uint128_t, FF> value;
};

struct Instruction {
    WireOpCode opcode;
    uint16_t indirect;
    std::vector<Operand> operands;
    uint8_t size_in_bytes;

    bool operator==(const Instruction& other) const = default;
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
