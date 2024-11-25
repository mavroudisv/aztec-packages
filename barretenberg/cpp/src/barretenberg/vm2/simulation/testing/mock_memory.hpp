#pragma once

#include <gmock/gmock.h>

#include "barretenberg/vm2/common/memory_types.hpp"
#include "barretenberg/vm2/simulation/memory.hpp"

namespace bb::avm::simulation {

class MockMemory : public MemoryInterface {
  public:
    MOCK_METHOD(void, set, (MemoryAddress index, MemoryValue value), (override));
    MOCK_METHOD(int, get, (MemoryAddress index), (const, override));
};

} // namespace bb::avm::simulation