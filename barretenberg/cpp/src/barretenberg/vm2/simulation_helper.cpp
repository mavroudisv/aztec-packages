#include "barretenberg/vm2/simulation_helper.hpp"

#include <list>

#include "barretenberg/vm/avm/trace/gadgets/range_check.hpp"
#include "barretenberg/vm2/simulation/addressing.hpp"
#include "barretenberg/vm2/simulation/alu.hpp"
#include "barretenberg/vm2/simulation/context.hpp"
#include "barretenberg/vm2/simulation/execution.hpp"

namespace bb::avm {

using namespace bb::avm::simulation;

EventsContainer AvmSimulationHelper::simulate()
{
    // Simulate.
    EventEmitter<ExecutionEvent> execution_emitter;
    EventEmitter<AluEvent> alu_emitter;
    EventEmitter<MemoryEvent> memory_emitter;
    EventEmitter<AddressingEvent> addressing_emitter;

    Alu alu(alu_emitter);
    Addressing addressing(addressing_emitter);
    ContextProvider context_provider(memory_emitter);
    Execution execution(alu, addressing, context_provider, execution_emitter);

    execution.run();

    // Print events.
    for (const auto& event : execution_emitter.get_events()) {
        std::cout << event << std::endl;
    }

    return { execution_emitter.dump_events(),
             alu_emitter.dump_events(),
             memory_emitter.dump_events(),
             addressing_emitter.dump_events() };
}

} // namespace bb::avm