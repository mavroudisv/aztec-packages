#pragma once

#include <list>

#include "barretenberg/vm2/simulation/events/events_container.hpp"

namespace bb::avm {

class AvmSimulationHelper {
  public:
    AvmSimulationHelper() = default;

    simulation::EventsContainer simulate();
};

} // namespace bb::avm