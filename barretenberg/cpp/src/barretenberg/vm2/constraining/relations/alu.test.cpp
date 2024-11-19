#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdint>

#include "barretenberg/vm2/constraining/relations/alu.hpp"
#include "barretenberg/vm2/constraining/testing/check_relation.hpp"
#include "barretenberg/vm2/generated/flavor_settings.hpp"
#include "barretenberg/vm2/testing/macros.hpp"
#include "barretenberg/vm2/tracegen/test_trace_container.hpp"

namespace bb::avm::constraining {
namespace {

using tracegen::TestTraceContainer;
using FF = AvmFlavorSettings::FF;
using C = Column;

TEST(AvmConstrainingTest, AluPositive)
{
    TestTraceContainer::RowTraceContainer trace = {
        { .alu_sel_op_add = 1, .alu_a = 1, .alu_b = 2, .alu_c = 3 },
    };

    check_relation<alu<FF>>(trace);
}

TEST(AvmConstrainingTest, AluNegativeBoolean)
{
    TestTraceContainer::RowTraceContainer trace = {
        // Negative test, this should be a boolean only!
        { .alu_sel_op_add = 23 },
    };

    EXPECT_THROW_WITH_MESSAGE(check_relation<alu<FF>>(trace, 0), "subrelation 0");
}

TEST(AvmConstrainingTest, AluNegativeAdd)
{
    TestTraceContainer::RowTraceContainer trace = {
        {
            // Observe that I'm making subrelation 0 fail too, but we'll only check subrelation 1!
            .alu_sel_op_add = 1,
            // Wrong ADD.
            .alu_a = 1,
            .alu_b = 1,
            .alu_c = 0
            //
        },
    };

    EXPECT_THROW_WITH_MESSAGE(check_relation<alu<FF>>(trace, 1), "subrelation 1");
}

} // namespace
} // namespace bb::avm::constraining