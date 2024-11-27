#pragma once

#include <gmock/gmock.h>

#include "barretenberg/vm2/common/memory_types.hpp"
#include "barretenberg/vm2/simulation/events/memory_event.hpp"
#include "barretenberg/vm2/simulation/memory.hpp"

namespace bb::avm::simulation {

class MockMemory : public MemoryInterface {
  public:
    // https://google.github.io/googletest/gmock_cook_book.html#making-the-compilation-faster
    MockMemory();
    ~MockMemory() override;

    MOCK_METHOD(void, set, (MemoryAddress index, MemoryValue value, MemoryTag tag), (override));
    MOCK_METHOD(ValueAndTag, get, (MemoryAddress index), (const, override));
    MOCK_METHOD((std::pair<std::vector<MemoryValue>, std::vector<MemoryTag>>),
                get_slice,
                (MemoryAddress start, size_t size),
                (const, override));
    MOCK_METHOD(uint32_t, get_space_id, (), (const, override));
};

} // namespace bb::avm::simulation