#pragma once

#include <list>

#include "barretenberg/vm2/simulation/events/addressing_event.hpp"
#include "barretenberg/vm2/simulation/events/alu_event.hpp"
#include "barretenberg/vm2/simulation/events/execution_event.hpp"
#include "barretenberg/vm2/simulation/events/memory_event.hpp"

namespace bb::avm::simulation {

struct EventsContainer {
    std::list<ExecutionEvent> execution;
    std::list<AluEvent> alu;
    std::list<MemoryEvent> memory;
    std::list<AddressingEvent> addressing;
};

} // namespace bb::avm::simulation