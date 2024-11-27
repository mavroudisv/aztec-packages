#pragma once

#include "barretenberg/vm2/simulation/events/events_container.hpp"
#include "barretenberg/vm2/tracegen/trace_container.hpp"

namespace bb::avm {

class AvmTraceGenHelper {
  public:
    AvmTraceGenHelper() = default;

    tracegen::TraceContainer generate_trace(simulation::EventsContainer& events);
};

} // namespace bb::avm