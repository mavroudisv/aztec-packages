#pragma once

#include <cstddef>
#include <gmock/gmock.h>

#include <array>
#include <memory>
#include <vector>

#include "barretenberg/vm2/common/memory_types.hpp"
#include "barretenberg/vm2/simulation/addressing.hpp"
#include "barretenberg/vm2/simulation/memory.hpp"

namespace bb::avm::simulation {

class MockAddressing : public AddressingBase {
  public:
    MockAddressing() = default;
    MOCK_METHOD(std::vector<MemoryAddress>,
                resolve,
                (uint16_t indirect,
                 const std::vector<MemoryAddress>& offsets,
                 size_t num_addreses_to_resolve,
                 MemoryInterface& memory),
                (const override));
};

} // namespace bb::avm::simulation