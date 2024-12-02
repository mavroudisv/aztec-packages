#include "barretenberg/vm2/simulation/bytecode_manager.hpp"

#include "barretenberg/common/serialize.hpp"
#include "barretenberg/crypto/poseidon2/poseidon2.hpp"
#include "barretenberg/crypto/poseidon2/poseidon2_params.hpp"
#include "barretenberg/vm2/common/aztec_types.hpp"
#include "barretenberg/vm2/simulation/lib/serialization.hpp"

namespace bb::avm::simulation {

BytecodeId TxBytecodeManager::get_bytecode(const AztecAddress& address)
{
    // TODO: in principle we want to do this, but we can't make hints fail. Think about it.
    // auto it = resolved_addresses.find(address);
    // if (it != resolved_addresses.end()) {
    //     return it->second;
    // }

    // TODO: catch errors etc.
    // TODO: we should trigger the proper merkle checks etc. The raw DB doesn't.
    ContractInstance instance = db.get_contract_instance(address);
    ContractClass klass = db.get_contract_class(instance.contract_class_id);
    FF hash = compute_public_bytecode_commitment(klass.packed_bytecode);

    // OK OKKKK *maybe* it makes sense to use a shared_ptr with bytecode.
    hash_events.emit({ .class_id = instance.contract_class_id, .bytecode = klass.packed_bytecode, .hash = hash });

    // We now save the bytecode so that we don't repeat this process.
    auto bytecode_id = next_bytecode_id++;
    resolved_addresses[address] = bytecode_id;
    bytecodes.emplace(
        bytecode_id,
        BytecodeInfo{ .bytecode = std::move(klass.packed_bytecode), .class_id = instance.contract_class_id });

    return bytecode_id;
}

std::pair<Instruction, uint32_t> TxBytecodeManager::read_instruction(BytecodeId bytecode_id, uint32_t pc)
{
    auto it = bytecodes.find(bytecode_id);
    if (it == bytecodes.end()) {
        throw std::runtime_error("Bytecode not found");
    }

    const auto& bytecode = it->second.bytecode;
    // TODO: catch errors etc.
    auto instruction_and_bytes = decode_instruction(bytecode, pc);

    decomposition_events.emit(
        { .class_id = it->second.class_id, .pc = pc, .instruction = instruction_and_bytes.first });

    return instruction_and_bytes;
}

ContractClassId TxBytecodeManager::get_class_id(BytecodeId bytecode_id) const
{
    auto it = bytecodes.find(bytecode_id);
    if (it == bytecodes.end()) {
        throw std::runtime_error("Bytecode not found");
    }

    return it->second.class_id;
}

FF TxBytecodeManager::compute_public_bytecode_commitment(std::span<const uint8_t> bytecode)
{
    auto encode_bytecode = [](std::span<const uint8_t> contract_bytes) -> std::vector<FF> {
        // To make from_buffer<uint256_t> work properly, we need to make sure the contract is a multiple of 31 bytes
        // Otherwise we will end up over-reading the buffer
        size_t padded_size = 31 * ((contract_bytes.size() + 30) / 31);
        // We dont want to mutate the original contract bytes, since we will (probably) need them later in the trace
        // unpadded
        std::vector<uint8_t> contract_bytes_padded(contract_bytes.begin(), contract_bytes.end());
        contract_bytes_padded.resize(padded_size, 0);
        std::vector<FF> contract_bytecode_fields;
        for (size_t i = 0; i < contract_bytes_padded.size(); i += 31) {
            uint256_t u256_elem = from_buffer<uint256_t>(contract_bytes_padded, i);
            // Drop the last byte
            contract_bytecode_fields.emplace_back(u256_elem >> 8);
        }
        return contract_bytecode_fields;
    };

    std::vector<FF> contract_bytecode_fields = encode_bytecode(bytecode);
    FF running_hash = 0;
    for (const auto& contract_bytecode_field : contract_bytecode_fields) {
        using poseidon2 = crypto::Poseidon2<crypto::Poseidon2Bn254ScalarFieldParams>;
        running_hash = poseidon2::hash({ contract_bytecode_field, running_hash });
    }
    return running_hash;
}

} // namespace bb::avm::simulation