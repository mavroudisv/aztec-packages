#include "gmock/gmock.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "barretenberg/vm2/simulation/events/event_emitter.hpp"
#include "barretenberg/vm2/simulation/events/execution_event.hpp"
#include "barretenberg/vm2/simulation/execution.hpp"
#include "barretenberg/vm2/simulation/testing/mock_addressing.hpp"
#include "barretenberg/vm2/simulation/testing/mock_alu.hpp"
#include "barretenberg/vm2/simulation/testing/mock_memory.hpp"

namespace bb::avm::simulation {
namespace {

using ::testing::_;
using ::testing::ElementsAre;
using ::testing::Return;

TEST(AvmSimulationExecutionTest, Add)
{
    MockAlu alu;
    MockMemory memory;
    MockAddressing addressing;

    EXPECT_CALL(addressing, resolve_(/*indirect=*/0, std::vector<uint32_t>{ 1, 2, 3 }, _))
        .WillOnce(Return(std::vector<uint32_t>{ 1, 2, 3 }));
    EXPECT_CALL(alu, add(1, 2, 3));

    EventEmitter<ExecutionEvent> execution_event_emitter;
    Execution execution(memory, alu, addressing, execution_event_emitter);
    execution.add(1, 2, 3, 0);

    EXPECT_THAT(execution_event_emitter.get_events(),
                ElementsAre(ExecutionEvent{
                    .opcode = ExecutionOpCode::ADD,
                    .indirect = 0,
                    .operands = { 1, 2, 3 },
                    .resolved_operands = { 1, 2, 3 },
                }));
}

} // namespace
} // namespace bb::avm::simulation