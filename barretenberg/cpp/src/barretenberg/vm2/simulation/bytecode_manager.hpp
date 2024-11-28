#pragma once

#include <cstdint>
#include <span>
#include <utility>
#include <vector>

#include "barretenberg/vm2/simulation/serialization.hpp"

namespace bb::avm::simulation {

class BytecodeManagerInterface {
  public:
    virtual ~BytecodeManagerInterface() = default;

    virtual std::pair<Instruction, /*read_bytes*/ uint32_t> read_instruction(size_t pc) const = 0;
};

// Manages a single bytecode instance.
// TODO: this will be emitting decomposition events.
class BytecodeManager : public BytecodeManagerInterface {
  public:
    BytecodeManager(std::span<const uint8_t> bytecode)
        : bytecode(bytecode.begin(), bytecode.end())
    {}

    std::pair<Instruction, /*read_bytes*/ uint32_t> read_instruction(size_t pc) const override;

  private:
    std::vector<uint8_t> bytecode;
};

} // namespace bb::avm::simulation