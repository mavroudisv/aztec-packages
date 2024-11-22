#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdint>

#include "barretenberg/vm2/constraining/relations/cpu.hpp"
#include "barretenberg/vm2/constraining/testing/check_relation.hpp"
#include "barretenberg/vm2/generated/flavor_settings.hpp"
#include "barretenberg/vm2/testing/macros.hpp"
#include "barretenberg/vm2/tracegen/test_trace_container.hpp"

namespace bb::avm::constraining {
namespace {

using tracegen::TestTraceContainer;
using FF = AvmFlavorSettings::FF;
using C = Column;

TEST(AvmConstrainingTest, CpuPositive)
{
    // clang-format off
    TestTraceContainer trace({
        {{ C::pc, 0 }},
        {{ C::pc, 1 }, { C::last, 1 }}
    });
    // clang-format on

    check_relation<cpu<FF>>(trace.as_rows());
}

TEST(AvmConstrainingTest, CpuNegativePc)
{
    // clang-format off
    TestTraceContainer trace({
        {{ C::pc, 0 }},
        {{ C::pc, 2 }, { C::last, 1 }}
    });
    // clang-format on

    EXPECT_THROW_WITH_MESSAGE(check_relation<cpu<FF>>(trace.as_rows(), 0), "subrelation 0.*row 0");
}

} // namespace
} // namespace bb::avm::constraining