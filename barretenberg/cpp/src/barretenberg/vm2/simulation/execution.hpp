#pragma once

#include <cstdint>
#include <memory>
#include <stack>

#include "barretenberg/vm2/common/memory_types.hpp"
#include "barretenberg/vm2/simulation/addressing.hpp"
#include "barretenberg/vm2/simulation/alu.hpp"
#include "barretenberg/vm2/simulation/context.hpp"
#include "barretenberg/vm2/simulation/events/event_emitter.hpp"
#include "barretenberg/vm2/simulation/events/execution_event.hpp"
#include "barretenberg/vm2/simulation/memory.hpp"

namespace bb::avm::simulation {

class Execution final {
  public:
    Execution(AluInterface& alu,
              AddressingBase& addressing,
              ContextProviderInterface& context_provider,
              EventEmitterInterface<ExecutionEvent>& event_emitter)
        : alu(alu)
        , addressing(addressing)
        , context_provider(context_provider)
        , events(event_emitter)
    {}

    void enter_context(std::unique_ptr<Context> context) { context_stack.push(std::move(context)); }

    void add(MemoryAddress a_operand, MemoryAddress b_operand, MemoryAddress dst_operand, uint8_t indirect);
    void call(MemoryAddress addr_operand, uint8_t indirect);

  private:
    Context& current_context() { return *context_stack.top(); }

    std::stack<std::unique_ptr<Context>> context_stack;

    AluInterface& alu;
    AddressingBase& addressing;
    [[maybe_unused]] ContextProviderInterface& context_provider;
    EventEmitterInterface<ExecutionEvent>& events;
};

} // namespace bb::avm::simulation