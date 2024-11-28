#pragma once

#include <gmock/gmock.h>

#include <cstddef>
#include <memory>
#include <span>
#include <vector>

#include "barretenberg/vm2/common/memory_types.hpp"
#include "barretenberg/vm2/simulation/addressing.hpp"
#include "barretenberg/vm2/simulation/memory.hpp"

namespace bb::avm::simulation {

class MockAddressing : public AddressingBase {
  public:
    // https://google.github.io/googletest/gmock_cook_book.html#making-the-compilation-faster
    MockAddressing();
    ~MockAddressing() override;

    MOCK_METHOD(std::vector<MemoryAddress>,
                resolve,
                (uint16_t indirect, std::span<const MemoryAddress> offsets, MemoryInterface& memory),
                (const override));
};

} // namespace bb::avm::simulation