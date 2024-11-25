#include <gmock/gmock.h>

#include "barretenberg/vm2/simulation/alu.hpp"

namespace bb::avm::simulation {

class MockAlu : public AluInterface {
  public:
    MOCK_METHOD(void, add, (uint32_t a_addr, uint32_t b_addr, uint32_t dst_addr), (override));
};

} // namespace bb::avm::simulation