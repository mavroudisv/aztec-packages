#pragma once

#include <cstdint>
#include <memory>

#include <gmock/gmock.h>

#include "barretenberg/vm2/simulation/context.hpp"
#include "barretenberg/vm2/simulation/memory.hpp"

namespace bb::avm::simulation {

class MockContextProvider : public ContextProviderInterface {
  public:
    MOCK_METHOD(std::unique_ptr<Context>, make, (std::unique_ptr<MemoryInterface> memory), (const, override));
};

} // namespace bb::avm::simulation