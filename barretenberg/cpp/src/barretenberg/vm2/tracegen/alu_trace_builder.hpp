#pragma once

#include <list>

#include "barretenberg/vm2/generated/columns.hpp"
#include "barretenberg/vm2/simulation/events/alu_event.hpp"
#include "barretenberg/vm2/tracegen/trace_container.hpp"

namespace bb::avm::tracegen {

class AluTraceBuilder final {
  public:
    // TODO: do not take a list here. C++ doesnt just let me take any iterable.
    void process(const std::list<simulation::AluEvent>& events, TraceContainer& trace);
};

} // namespace bb::avm::tracegen