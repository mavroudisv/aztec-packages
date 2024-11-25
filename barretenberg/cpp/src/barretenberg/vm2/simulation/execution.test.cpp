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

TEST(AvmSimulationExecutionTest, Add)
{
    MockAlu alu;
    MockAddressing addressing;
    MockContextProvider context_provider;

    EXPECT_CALL(addressing, resolve_(/*indirect=*/0, std::vector<MemoryAddress>{ 1, 2, 3 }, _))
        .WillOnce(Return(std::vector<MemoryAddress>{ 4, 5, 6 }));
    EXPECT_CALL(alu, add(4, 5, 6));

    auto context = std::make_unique<Context>(std::make_unique<MockMemory>());

    EventEmitter<ExecutionEvent> execution_event_emitter;
    Execution execution(alu, addressing, context_provider, execution_event_emitter);
    execution.enter_context(std::move(context));
    execution.add(1, 2, 3, 0);

    EXPECT_THAT(execution_event_emitter.get_events(),
                ElementsAre(ExecutionEvent{
                    .opcode = ExecutionOpCode::ADD,
                    .indirect = 0,
                    .operands = { 1, 2, 3 },
                    .resolved_operands = { 4, 5, 6 },
                }));
}

} // namespace
} // namespace bb::avm::simulation