#include "barretenberg/vm2/simulation/bytecode_manager.hpp"

#include "barretenberg/vm2/common/aztec_types.hpp"
#include "barretenberg/vm2/simulation/serialization.hpp"

namespace bb::avm::simulation {

BytecodeId TxBytecodeManager::get_bytecode(const AztecAddress& address)
{
    auto it = resolved_addresses.find(address);
    if (it != resolved_addresses.end()) {
        return it->second;
    }

    // TODO: Resolve, fetch, etc.
    auto bytecode = std::vector<uint8_t>();

    // TODO: Hash bytecode.

    // We now save the bytecode so that we don't repeat this process.
    ContractClassId class_id = address; // TODO: trigger calculation.
    auto bytecode_id = next_bytecode_id++;
    resolved_addresses[address] = bytecode_id;
    bytecodes.emplace(
        std::make_pair(bytecode_id, BytecodeInfo{ .bytecode = std::move(bytecode), .class_id = class_id }));

    return bytecode_id;
}

std::pair<Instruction, uint32_t> TxBytecodeManager::read_instruction(BytecodeId bytecode_id, size_t pc)
{
    auto it = bytecodes.find(bytecode_id);
    if (it == bytecodes.end()) {
        throw std::runtime_error("Bytecode not found");
    }

    // TODO: trigger decomposition

    const auto& bytecode = it->second.bytecode;
    return decode_instruction(bytecode, pc);
}

ContractClassId TxBytecodeManager::get_class_id(BytecodeId bytecode_id) const
{
    auto it = bytecodes.find(bytecode_id);
    if (it == bytecodes.end()) {
        throw std::runtime_error("Bytecode not found");
    }

    return it->second.class_id;
}

} // namespace bb::avm::simulation