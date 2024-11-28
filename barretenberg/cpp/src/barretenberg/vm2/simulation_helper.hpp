#pragma once

#include <list>

#include "barretenberg/vm2/simulation/events/events_container.hpp"

namespace bb::avm {

class AvmSimulationHelper {
  public:
    AvmSimulationHelper() = default;

    // Full simulation with event collection.
    simulation::EventsContainer simulate();

    // Fast simulation without event collection.
    void simulate_fast();
};

} // namespace bb::avm