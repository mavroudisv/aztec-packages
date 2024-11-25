#pragma once

#include <gmock/gmock.h>

#include "barretenberg/vm2/common/memory_types.hpp"
#include "barretenberg/vm2/simulation/alu.hpp"

namespace bb::avm::simulation {

class MockAlu : public AluInterface {
  public:
    MOCK_METHOD(void, add, (MemoryAddress a_addr, MemoryAddress b_addr, MemoryAddress dst_addr), (override));
};

} // namespace bb::avm::simulation