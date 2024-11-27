#include "barretenberg/vm2/avm_api.hpp"

#include "barretenberg/vm2/simulation_helper.hpp"
#include "barretenberg/vm2/tracegen_helper.hpp"

namespace bb::avm {

using namespace bb::avm::simulation;

std::tuple<AvmAPI::AvmProof, AvmAPI::AvmVerificationKey> AvmAPI::prove()
{
    // Simulate.
    AvmSimulationHelper simulation_helper;
    auto events = simulation_helper.simulate();

    // Generate trace.
    AvmTraceGenHelper tracegen_helper;
    auto trace = tracegen_helper.generate_trace(events);

    // Prove.
    // TBD: Implement proving. This depends on the flavor.

    return { AvmProof(), 0 };
}

} // namespace bb::avm