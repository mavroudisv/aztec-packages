#include "barretenberg/vm2/simulation/execution.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "barretenberg/vm2/common/field.hpp"
#include "barretenberg/vm2/common/memory_types.hpp"
#include "barretenberg/vm2/simulation/context.hpp"
#include "barretenberg/vm2/simulation/events/event_emitter.hpp"
#include "barretenberg/vm2/simulation/events/execution_event.hpp"
#include "barretenberg/vm2/simulation/memory.hpp"
#include "barretenberg/vm2/simulation/testing/mock_addressing.hpp"
#include "barretenberg/vm2/simulation/testing/mock_alu.hpp"
#include "barretenberg/vm2/simulation/testing/mock_context.hpp"
#include "barretenberg/vm2/simulation/testing/mock_memory.hpp"

namespace bb::avm::simulation {
namespace {

using ::testing::_;
using ::testing::Ref;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;

class AvmSimulationExecutionTest : public ::testing::Test {
  protected:
    AvmSimulationExecutionTest() { ON_CALL(context, get_memory).WillByDefault(ReturnRef(memory)); }

    StrictMock<MockAlu> alu;
    StrictMock<MockAddressing> addressing;
    StrictMock<MockMemory> memory;
    StrictMock<MockContextProvider> context_provider;
    std::unique_ptr<StrictMock<MockContext>> context_obj = std::make_unique<StrictMock<MockContext>>();
    StrictMock<MockContext>& context = *context_obj;
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
    MemoryAddress ret_offset = 1;
    MemoryAddress ret_size_offset = 2;
    size_t ret_size = 7;
    std::vector<FF> returndata = { 1, 2, 3, 4, 5 };

    MockMemory child_memory;
    auto child_context_obj = std::make_unique<MockContext>();
    auto& child_context = *child_context_obj;
    EXPECT_CALL(child_context, get_memory).WillRepeatedly(ReturnRef(child_memory));
    EXPECT_CALL(child_memory, get(ret_size_offset)).WillOnce(Return<ValueAndTag>({ ret_size, MemoryTag::U32 }));
    EXPECT_CALL(child_memory, get_slice(ret_offset, ret_size)).WillOnce(Return<SliceWithTags>({ returndata, {} }));

    // FIX: i'm not checking popping of context, etc. And it's not easy to do.
    // This might mean that something in the design is not ok.
    EXPECT_CALL(/*parent_context*/ context, set_nested_returndata(returndata));

    execution.enter_context(std::move(context_obj));
    execution.enter_context(std::move(child_context_obj));
    execution.ret(child_context, 1, 2);
}

} // namespace
} // namespace bb::avm::simulation