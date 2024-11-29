#include "barretenberg/vm2/simulation/execution.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "barretenberg/vm2/common/field.hpp"
#include "barretenberg/vm2/common/memory_types.hpp"
#include "barretenberg/vm2/common/opcodes.hpp"
#include "barretenberg/vm2/simulation/context.hpp"
#include "barretenberg/vm2/simulation/events/event_emitter.hpp"
#include "barretenberg/vm2/simulation/events/execution_event.hpp"
#include "barretenberg/vm2/simulation/memory.hpp"
#include "barretenberg/vm2/simulation/serialization.hpp"
#include "barretenberg/vm2/simulation/testing/mock_addressing.hpp"
#include "barretenberg/vm2/simulation/testing/mock_alu.hpp"
#include "barretenberg/vm2/simulation/testing/mock_bytecode_manager.hpp"
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
    FF ret_size_ff = ret_size;
    std::vector<FF> returndata = { 1, 2, 3, 4, 5 };

    MockMemory child_memory;
    auto child_context_obj = std::make_unique<MockContext>();
    auto& child_context = *child_context_obj;
    EXPECT_CALL(child_context, get_memory).WillRepeatedly(ReturnRef(child_memory));
    EXPECT_CALL(child_memory, get(ret_size_offset)).WillOnce(Return<ValueRefAndTag>({ ret_size_ff, MemoryTag::U32 }));
    EXPECT_CALL(child_memory, get_slice(ret_offset, ret_size)).WillOnce(Return<SliceWithTags>({ returndata, {} }));

    // FIX: i'm not checking popping of context, etc. And it's not easy to do.
    // This might mean that something in the design is not ok.
    EXPECT_CALL(/*parent_context*/ context, set_nested_returndata(returndata));

    execution.enter_context(std::move(context_obj));
    execution.enter_context(std::move(child_context_obj));
    execution.ret(child_context, 1, 2);
}

// FIXME: Way too long and complicated.
TEST_F(AvmSimulationExecutionTest, Run)
{
    GTEST_SKIP(); // FIXME: Way too long and complicated.

    MockBytecodeManager bytecode_manager;
    EXPECT_CALL(context, get_bytecode_manager).WillRepeatedly(ReturnRef(bytecode_manager));
    EXPECT_CALL(context, get_memory).Times(2);

    // First instruction is an ADD_8.
    Instruction add8(WireOpCode::ADD_8, 0, { Operand::u8(1), Operand::u8(2), Operand::u8(3) });
    EXPECT_CALL(context, get_pc).WillOnce(Return(0));
    EXPECT_CALL(bytecode_manager, read_instruction(0))
        .WillOnce(Return(std::pair<Instruction, uint32_t>(add8, /*bytes_read*/ 10)));
    EXPECT_CALL(context, set_next_pc(10));
    EXPECT_CALL(addressing, resolve).WillOnce(Return(std::vector<MemoryAddress>({ 4, 5, 6 })));
    EXPECT_CALL(alu, add);
    EXPECT_CALL(context, get_next_pc).WillOnce(Return(10));
    EXPECT_CALL(context, set_pc(10));

    // Then we just return.
    Instruction ret(WireOpCode::RETURN, 0, { Operand::u16(1), Operand::u16(2) });
    EXPECT_CALL(context, get_pc).WillOnce(Return(10));
    EXPECT_CALL(bytecode_manager, read_instruction(10))
        .WillOnce(Return(std::pair<Instruction, uint32_t>(ret, /*bytes_read*/ 5)));
    EXPECT_CALL(context, set_next_pc(15));
    EXPECT_CALL(addressing, resolve).WillOnce(Return(std::vector<MemoryAddress>({ 2, 1 })));
    FF zero = 0; // ugh
    EXPECT_CALL(memory, get(1)).WillOnce(Return(ValueRefAndTag({ zero, MemoryTag::U32 })));
    EXPECT_CALL(memory, get_slice(2, 0)).WillOnce(Return(SliceWithTags({}, {})));
    EXPECT_CALL(context, get_next_pc).WillOnce(Return(18));
    EXPECT_CALL(context, set_pc(18));

    execution.enter_context(std::move(context_obj));
    execution.run();
}

} // namespace
} // namespace bb::avm::simulation