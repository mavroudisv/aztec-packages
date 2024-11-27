#pragma once

#include <cstdint>
#include <memory>
#include <stack>

#include "barretenberg/vm2/common/field.hpp"
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

    void run();

    void add(ContextInterface& context, MemoryAddress a_addr, MemoryAddress b_addr, MemoryAddress dst_addr);
    void jumpi(ContextInterface& context, uint32_t loc, MemoryAddress cond_addr);
    void call(ContextInterface& context, MemoryAddress addr);
    void ret(ContextInterface& context, MemoryAddress ret_offset, MemoryAddress ret_size_offset);

  private:
    ContextInterface& current_context() { return *context_stack.top(); }
    void enter_context(std::unique_ptr<ContextInterface> context) { context_stack.push(std::move(context)); }
    void pop_context(std::vector<FF>&& return_data);

    void dispatch_opcode(ExecutionOpCode opcode, const std::vector<MemoryAddress>& resolved_operands);
    template <typename... Ts>
    void call_with_operands(void (Execution::*f)(ContextInterface&, Ts...),
                            const std::vector<MemoryAddress>& resolved_operands);

    std::stack<std::unique_ptr<ContextInterface>> context_stack;

    AluInterface& alu;
    AddressingBase& addressing;
    ContextProviderInterface& context_provider;
    EventEmitterInterface<ExecutionEvent>& events;
};

} // namespace bb::avm::simulation