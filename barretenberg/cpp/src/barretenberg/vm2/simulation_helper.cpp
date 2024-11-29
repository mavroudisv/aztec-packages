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

// Configuration for full simulation (for proving).
struct RealEmitterSettings {
    using ExecutionEventEmitter = EventEmitter<ExecutionEvent>;
    using AluEventEmitter = EventEmitter<AluEvent>;
    using MemoryEventEmitter = EventEmitter<MemoryEvent>;
    using AddressingEventEmitter = EventEmitter<AddressingEvent>;
};

// Configuration for fast simulation.
struct NoopEmitterSettings {
    using ExecutionEventEmitter = NoopEventEmitter<ExecutionEvent>;
    using AluEventEmitter = NoopEventEmitter<AluEvent>;
    using MemoryEventEmitter = NoopEventEmitter<MemoryEvent>;
    using AddressingEventEmitter = NoopEventEmitter<AddressingEvent>;
};

template <typename S> EventsContainer simulate_with_emitter()
{
    // Simulate.
    typename S::ExecutionEventEmitter execution_emitter;
    typename S::AluEventEmitter alu_emitter;
    typename S::MemoryEventEmitter memory_emitter;
    typename S::AddressingEventEmitter addressing_emitter;

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
    return simulate_with_emitter<RealEmitterSettings>();
}

void AvmSimulationHelper::simulate_fast()
{
    simulate_with_emitter<NoopEmitterSettings>();
}

} // namespace bb::avm