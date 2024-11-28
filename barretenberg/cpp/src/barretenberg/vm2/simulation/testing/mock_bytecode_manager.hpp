#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

#include <gmock/gmock.h>

#include "barretenberg/vm2/simulation/bytecode_manager.hpp"

namespace bb::avm::simulation {

class MockBytecodeManager : public BytecodeManagerInterface {
  public:
    // https://google.github.io/googletest/gmock_cook_book.html#making-the-compilation-faster
    MockBytecodeManager();
    ~MockBytecodeManager() override;

    MOCK_METHOD((std::pair<Instruction, /*read_bytes*/ uint32_t>), read_instruction, (size_t pc), (const, override));
};

} // namespace bb::avm::simulation