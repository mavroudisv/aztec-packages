#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "barretenberg/vm2/simulation/alu.hpp"
#include "barretenberg/vm2/simulation/events/event_emitter.hpp"
#include "barretenberg/vm2/simulation/events/memory_event.hpp"
#include "barretenberg/vm2/simulation/memory.hpp"

namespace bb::avm::simulation {
namespace {

TEST(AvmSimulationAluTest, Add)
{
    DiscardingEventEmitter<MemoryEvent> emitter;
    Memory mem(/*space_id=*/0, emitter);
    EventEmitter<AluEvent> alu_event_emitter;
    Alu alu(mem, alu_event_emitter);

    // TODO: actually can choose to mock, not even use a memory, check the events, etc.
    uint32_t a_addr = 0;
    uint32_t b_addr = 1;
    uint32_t dst_addr = 2;

    mem.set(a_addr, 1, MemoryTag::U32);
    mem.set(b_addr, 2, MemoryTag::U32);

    alu.add(a_addr, b_addr, dst_addr);

    auto c = mem.get(dst_addr);
    EXPECT_EQ(c.value, 3);
    EXPECT_EQ(c.tag, MemoryTag::U32);
}

} // namespace
} // namespace bb::avm::simulation