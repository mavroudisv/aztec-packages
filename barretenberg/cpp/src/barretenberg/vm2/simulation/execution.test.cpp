#include "gmock/gmock.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

#include "barretenberg/vm2/common/field.hpp"
#include "barretenberg/vm2/common/memory_types.hpp"
#include "barretenberg/vm2/simulation/context.hpp"
#include "barretenberg/vm2/simulation/events/event_emitter.hpp"
#include "barretenberg/vm2/simulation/events/execution_event.hpp"
#include "barretenberg/vm2/simulation/execution.hpp"
#include "barretenberg/vm2/simulation/testing/mock_addressing.hpp"
#include "barretenberg/vm2/simulation/testing/mock_alu.hpp"
#include "barretenberg/vm2/simulation/testing/mock_context.hpp"
#include "barretenberg/vm2/simulation/testing/mock_memory.hpp"

namespace bb::avm::simulation {
namespace {

using ::testing::_;
using ::testing::Ref;
using ::testing::ReturnRef;
using ::testing::StrictMock;

class AvmSimulationExecutionTest : public ::testing::Test {
  protected:
    AvmSimulationExecutionTest() { ON_CALL(context, get_memory).WillByDefault(ReturnRef(memory)); }

    StrictMock<MockAlu> alu;
    StrictMock<MockAddressing> addressing;
    StrictMock<MockMemory> memory;
    StrictMock<MockContextProvider> context_provider;
    StrictMock<MockContext> context;
    EventEmitter<ExecutionEvent> execution_event_emitter;
    Execution execution = Execution(alu, addressing, context_provider, execution_event_emitter);
};

TEST_F(AvmSimulationExecutionTest, Add)
{
    EXPECT_CALL(alu, add(Ref(context), 4, 5, 6));
    execution.add(context, 4, 5, 6);
}

TEST_F(AvmSimulationExecutionTest, Return)
{
    // auto& parent_context = context;
    // auto child_context = std::make_unique<MockContext>();
    // std::vector<FF> returndata = { 1, 2, 3, 4, 5 };

    // EXPECT_CALL(parent_context, set_nested_returndata(returndata));
}

} // namespace
} // namespace bb::avm::simulation