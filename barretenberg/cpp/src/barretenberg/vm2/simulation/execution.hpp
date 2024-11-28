#pragma once

#include <cstdint>
#include <memory>
#include <span>
#include <stack>
#include <vector>

#include "barretenberg/vm2/common/aztec_types.hpp"
#include "barretenberg/vm2/common/field.hpp"
#include "barretenberg/vm2/common/memory_types.hpp"
#include "barretenberg/vm2/common/opcodes.hpp"
#include "barretenberg/vm2/simulation/addressing.hpp"
#include "barretenberg/vm2/simulation/alu.hpp"
#include "barretenberg/vm2/simulation/context.hpp"
#include "barretenberg/vm2/simulation/events/event_emitter.hpp"
#include "barretenberg/vm2/simulation/events/execution_event.hpp"
#include "barretenberg/vm2/simulation/memory.hpp"
#include "barretenberg/vm2/simulation/serialization.hpp"

namespace bb::avm::simulation {

struct ExecutionResult {
    std::vector<FF> returndata;
    bool success;
};

class ExecutionInterface {
  public:
    virtual ~ExecutionInterface() = default;
    // Returns the top-level execution result.
    virtual ExecutionResult execute(AztecAddress contract_address,
                                    std::span<const FF> calldata,
                                    AztecAddress msg_sender,
                                    bool is_static) = 0;
};

// In charge of executing a single enqueued call.
class Execution : public ExecutionInterface {
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

    ExecutionResult execute(AztecAddress contract_address,
                            std::span<const FF> calldata,
                            AztecAddress msg_sender,
                            bool is_static) override;

    void enter_context(std::unique_ptr<ContextInterface> context) { context_stack.push(std::move(context)); }

    // Opcode handlers. The order of the operands matters and should be the same as the wire format.
    void add(ContextInterface& context, MemoryAddress a_addr, MemoryAddress b_addr, MemoryAddress dst_addr);
    void jumpi(ContextInterface& context, uint32_t loc, MemoryAddress cond_addr);
    void call(ContextInterface& context, MemoryAddress addr);
    void ret(ContextInterface& context, MemoryAddress ret_offset, MemoryAddress ret_size_offset);

  private:
    ContextInterface& current_context() { return *context_stack.top(); }
    void run();

    void dispatch_opcode(ExecutionOpCode opcode, const std::vector<Operand>& resolved_operands);
    template <typename... Ts>
    void call_with_operands(void (Execution::*f)(ContextInterface&, Ts...),
                            const std::vector<Operand>& resolved_operands);
    std::pair<ExecutionOpCode, std::vector<Operand>> resolve_instruction(const Instruction& instruction);

    std::stack<std::unique_ptr<ContextInterface>> context_stack;
    ExecutionResult top_level_result;

    AluInterface& alu;
    AddressingBase& addressing;
    ContextProviderInterface& context_provider;
    EventEmitterInterface<ExecutionEvent>& events;
};

} // namespace bb::avm::simulation