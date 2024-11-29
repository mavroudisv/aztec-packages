#include "barretenberg/vm2/simulation_helper.hpp"

#include <list>
#include <vector>

#include "barretenberg/common/log.hpp"
#include "barretenberg/vm/avm/trace/gadgets/range_check.hpp"
#include "barretenberg/vm2/common/aztec_types.hpp"
#include "barretenberg/vm2/common/field.hpp"
#include "barretenberg/vm2/simulation/addressing.hpp"
#include "barretenberg/vm2/simulation/alu.hpp"
#include "barretenberg/vm2/simulation/bytecode_manager.hpp"
#include "barretenberg/vm2/simulation/context.hpp"
#include "barretenberg/vm2/simulation/execution.hpp"
#include "barretenberg/vm2/simulation/tx_execution.hpp"

namespace bb::avm {

using namespace bb::avm::simulation;

namespace {

// Configuration for full simulation (for proving).
struct ProvingSettings {
    using ExecutionEventEmitter = EventEmitter<ExecutionEvent>;
    using AluEventEmitter = EventEmitter<AluEvent>;
    using MemoryEventEmitter = EventEmitter<MemoryEvent>;
    using AddressingEventEmitter = EventEmitter<AddressingEvent>;
    using BytecodeHashingEventEmitter = EventEmitter<BytecodeHashingEvent>;
    using BytecodeDecompositionEventEmitter = EventEmitter<BytecodeDecompositionEvent>;
};

// Configuration for fast simulation.
struct FastSettings {
    using ExecutionEventEmitter = NoopEventEmitter<ExecutionEvent>;
    using AluEventEmitter = NoopEventEmitter<AluEvent>;
    using MemoryEventEmitter = NoopEventEmitter<MemoryEvent>;
    using AddressingEventEmitter = NoopEventEmitter<AddressingEvent>;
    using BytecodeHashingEventEmitter = NoopEventEmitter<BytecodeHashingEvent>;
    using BytecodeDecompositionEventEmitter = NoopEventEmitter<BytecodeDecompositionEvent>;
};

template <typename S> EventsContainer simulate_with_settings()
{
    typename S::ExecutionEventEmitter execution_emitter;
    typename S::AluEventEmitter alu_emitter;
    typename S::MemoryEventEmitter memory_emitter;
    typename S::AddressingEventEmitter addressing_emitter;
    typename S::BytecodeHashingEventEmitter bytecode_hashing_emitter;
    typename S::BytecodeDecompositionEventEmitter bytecode_decomposition_emitter;

    Alu alu(alu_emitter);
    Addressing addressing(addressing_emitter);
    TxBytecodeManager bytecode_manager(bytecode_hashing_emitter, bytecode_decomposition_emitter);
    ContextProvider context_provider(bytecode_manager, memory_emitter);
    Execution execution(alu, addressing, context_provider, execution_emitter);
    TxExecution tx_execution(execution);

    std::vector<PublicExecutionRequest> enqueued_calls = {
        { AztecAddress(0xdeadbeef), AztecAddress(0), { 1, 2, 3, 4 }, false },
        { AztecAddress(0xdead0101), AztecAddress(0), { 1, 2, 3, 4 }, false },
    };
    tx_execution.simulate({ enqueued_calls });

    return { execution_emitter.dump_events(),        alu_emitter.dump_events(),
             memory_emitter.dump_events(),           addressing_emitter.dump_events(),
             bytecode_hashing_emitter.dump_events(), bytecode_decomposition_emitter.dump_events() };
}

} // namespace

EventsContainer AvmSimulationHelper::simulate()
{
    return simulate_with_settings<ProvingSettings>();
}

void AvmSimulationHelper::simulate_fast()
{
    simulate_with_settings<FastSettings>();
}

} // namespace bb::avm