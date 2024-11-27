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
using ::testing::Ref;
using ::testing::ReturnRef;

TEST(AvmSimulationExecutionTest, Add)
{
    MockAlu alu;
    MockAddressing addressing;
    MockContextProvider context_provider;
    MockContext context;

    EXPECT_CALL(alu, add(Ref(context), 4, 5, 6));

    EventEmitter<ExecutionEvent> execution_event_emitter;
    Execution execution(alu, addressing, context_provider, execution_event_emitter);
    execution.add(context, 4, 5, 6);
}

} // namespace
} // namespace bb::avm::simulation