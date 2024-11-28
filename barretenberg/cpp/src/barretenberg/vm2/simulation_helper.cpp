#include "barretenberg/vm2/simulation_helper.hpp"

#include <list>
#include <vector>

#include "barretenberg/common/log.hpp"
#include "barretenberg/vm/avm/trace/gadgets/range_check.hpp"
#include "barretenberg/vm2/common/aztec_types.hpp"
#include "barretenberg/vm2/common/field.hpp"
#include "barretenberg/vm2/simulation/addressing.hpp"
#include "barretenberg/vm2/simulation/alu.hpp"
#include "barretenberg/vm2/simulation/context.hpp"
#include "barretenberg/vm2/simulation/execution.hpp"
#include "barretenberg/vm2/simulation/tx_execution.hpp"

namespace bb::avm {

using namespace bb::avm::simulation;

namespace {

template <template <typename E> typename EventEmitterType> EventsContainer simulate_with_emitter()
{
    // Simulate.
    EventEmitterType<ExecutionEvent> execution_emitter;
    EventEmitterType<AluEvent> alu_emitter;
    EventEmitterType<MemoryEvent> memory_emitter;
    EventEmitterType<AddressingEvent> addressing_emitter;

    Alu alu(alu_emitter);
    Addressing addressing(addressing_emitter);
    ContextProvider context_provider(memory_emitter);
    Execution execution(alu, addressing, context_provider, execution_emitter);
    TransactionExecution tx_execution(execution);

    std::vector<PublicExecutionRequest> enqueued_calls = {
        { AztecAddress(0xdeadbeef), AztecAddress(0), { 1, 2, 3, 4 }, false },
        { AztecAddress(0xdead0101), AztecAddress(0), { 1, 2, 3, 4 }, false },
    };
    tx_execution.simulate({ enqueued_calls });

    return { execution_emitter.dump_events(),
             alu_emitter.dump_events(),
             memory_emitter.dump_events(),
             addressing_emitter.dump_events() };
}

} // namespace

EventsContainer AvmSimulationHelper::simulate()
{
    return simulate_with_emitter<EventEmitter>();
}

void AvmSimulationHelper::simulate_fast()
{
    simulate_with_emitter<NoopEventEmitter>();
}

} // namespace bb::avm