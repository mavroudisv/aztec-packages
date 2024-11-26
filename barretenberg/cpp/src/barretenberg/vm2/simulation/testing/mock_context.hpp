#pragma once

#include <cstdint>
#include <memory>

#include <gmock/gmock.h>

#include "barretenberg/vm2/simulation/context.hpp"
#include "barretenberg/vm2/simulation/memory.hpp"

namespace bb::avm::simulation {

class MockContext : public ContextInterface {
  public:
    MOCK_METHOD(MemoryInterface&, get_memory, (), (override));
    MOCK_METHOD(uint32_t, get_pc, (), (const, override));
    MOCK_METHOD(void, set_pc, (uint32_t new_pc), (override));
    MOCK_METHOD(uint32_t, get_next_pc, (), (const, override));
    MOCK_METHOD(void, set_next_pc, (uint32_t new_next_pc), (override));
};

class MockContextProvider : public ContextProviderInterface {
  public:
    MOCK_METHOD(std::unique_ptr<ContextInterface>, make, (int contract_address, uint32_t call_id), (const, override));
};

} // namespace bb::avm::simulation