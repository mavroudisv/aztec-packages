#include "barretenberg/vm2/simulation/addressing.hpp"

#include <algorithm>
#include <cstdint>
#include <vector>

#include "barretenberg/vm2/common/memory_types.hpp"
#include "barretenberg/vm2/simulation/events/addressing_event.hpp"
#include "barretenberg/vm2/simulation/events/event_emitter.hpp"
#include "barretenberg/vm2/simulation/events/memory_event.hpp"
#include "barretenberg/vm2/simulation/lib/serialization.hpp"
#include "barretenberg/vm2/simulation/memory.hpp"

namespace bb::avm::simulation {

// Will resolve all addresses from `offsets` using the memory interface.
std::vector<Operand> Addressing::resolve(const Instruction& instruction, MemoryInterface& memory) const
{
    // Note: it's fine to query instruction info in here since it does not trigger events.
    ExecutionOpCode opcode = instruction_info_db.map_wire_opcode_to_execution_opcode(instruction.opcode);
    const InstructionSpec& spec = instruction_info_db.get(opcode);
    assert(instruction.operands.size() <= spec.num_addresses);

    // Copy the original operands since they are our starting point.
    std::vector<Operand> resolved = instruction.operands;
    // TODO: propagate this error.
    assert(std::all_of(resolved.begin(), resolved.begin() + spec.num_addresses, [](const Operand&) {
        // MemoryAddress addr(o);
        // I don't have a tag so cannot check this now.
        // return memory.is_valid_address(addr);
        return true;
    }));

    // We retrieve, cache and check M[0] first because this is probably what we'll do in the circuit.
    auto stack_pointer = memory.get(0);
    // TODO: propagate this error.
    assert(memory.is_valid_address(stack_pointer));

    // First process relative addressing for all the addresses.
    for (size_t i = 0; i < spec.num_addresses; ++i) {
        MemoryAddress offset(resolved[i]);
        if ((offset >> i) & 1) {
            // TODO: check bounds and tags. See simulator for reference.
            offset += static_cast<MemoryAddress>(stack_pointer.value);
        }
        resolved[i] = static_cast<Operand>(offset);
    }
    auto after_relative = resolved;

    // Then indirection.
    for (size_t i = 0; i < spec.num_addresses; ++i) {
        MemoryAddress offset(resolved[i]);
        if ((offset >> (i + spec.num_addresses)) & 1) {
            // TODO: check bounds and tags. See simulator for reference.
            auto new_address = memory.get(offset);
            assert(memory.is_valid_address(new_address));
            offset = static_cast<MemoryAddress>(new_address.value);
        }
        resolved[i] = static_cast<Operand>(offset);
    }

    events.emit({ .instruction = std::move(instruction),
                  .after_relative = std::move(after_relative),
                  .resolved_operands = resolved,
                  .stack_pointer_val = stack_pointer.value,
                  .stack_pointer_tag = stack_pointer.tag,
                  .spec = spec });
    return resolved;
}

} // namespace bb::avm::simulation