#pragma once

#include <gmock/gmock.h>

#include <array>
#include <memory>
#include <vector>

#include "barretenberg/vm2/simulation/addressing.hpp"
#include "barretenberg/vm2/simulation/memory.hpp"

namespace bb::avm::simulation {

class MockAddressing final : public AddressingBase {
  public:
    MockAddressing() = default;
    MOCK_METHOD(std::vector<uint32_t>,
                resolve_,
                (uint16_t indirect, const std::vector<uint32_t>& offsets, MemoryInterface& memory),
                (const override));
};

} // namespace bb::avm::simulation