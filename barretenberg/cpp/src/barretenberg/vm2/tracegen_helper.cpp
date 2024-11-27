#include "barretenberg/vm2/tracegen_helper.hpp"

#include <list>

#include "barretenberg/vm2/tracegen/alu_trace_builder.hpp"
#include "barretenberg/vm2/tracegen/trace_container.hpp"

namespace bb::avm {

using namespace bb::avm::simulation;
using namespace bb::avm::tracegen;

TraceContainer AvmTraceGenHelper::generate_trace(EventsContainer& events)
{
    TraceContainer trace;

    AluTraceBuilder alu_builder;
    alu_builder.process(events.alu, trace);
    events.alu.clear();

    return trace;
}

} // namespace bb::avm