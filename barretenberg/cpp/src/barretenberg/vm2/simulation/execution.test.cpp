#include "gmock/gmock.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

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
using ::testing::ElementsAre;
using ::testing::Return;
using ::testing::ReturnRef;

TEST(AvmSimulationExecutionTest, Add)
{
    MockAlu alu;
    MockAddressing addressing;
    MockContextProvider context_provider;

    MockMemory memory;
    auto context = std::make_unique<MockContext>();
    EXPECT_CALL(*context, get_memory()).WillRepeatedly(ReturnRef(memory));
    // FIXME: it doesn't make sense to have to test/provide pc here.
    EXPECT_CALL(*context, get_pc()).WillRepeatedly(Return(0));

    EXPECT_CALL(alu, add(4, 5, 6));

    EventEmitter<ExecutionEvent> execution_event_emitter;
    Execution execution(alu, addressing, context_provider, execution_event_emitter);
    execution.enter_context(std::move(context));
    execution.add(4, 5, 6);
}

} // namespace
} // namespace bb::avm::simulation