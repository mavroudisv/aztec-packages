#include <gmock/gmock.h>

#include "barretenberg/vm2/simulation/memory.hpp"

namespace bb::avm::simulation {

class MockMemory : public MemoryInterface {
  public:
    MOCK_METHOD(void, set, (size_t index, int value), (override));
    MOCK_METHOD(int, get, (size_t index), (const, override));
};

} // namespace bb::avm::simulation