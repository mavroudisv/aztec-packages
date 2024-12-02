#pragma once

#include <vector>

#include "barretenberg/serialize/msgpack.hpp"
#include "barretenberg/vm2/common/aztec_types.hpp"
#include "barretenberg/vm2/common/field.hpp"

namespace bb::avm::simulation {

struct PublicKeysHint {
    AffinePoint nullifier_key;
    /** Incoming viewing public key */
    AffinePoint incoming_viewing_key;
    /** Outgoing viewing public key */
    AffinePoint outgoing_viewing_key;
    /** Tagging viewing public key */
    AffinePoint tagging_key;

    MSGPACK_FIELDS(nullifier_key, incoming_viewing_key, outgoing_viewing_key, tagging_key);
};

struct ContractInstanceHint {
    AztecAddress address;
    simulation::FF salt;
    AztecAddress deployer_addr;
    ContractClassId contract_class_id;
    simulation::FF initialisation_hash;
    PublicKeysHint public_keys;

    MSGPACK_FIELDS(address, salt, deployer_addr, contract_class_id, initialisation_hash, public_keys);
};

struct ContractClassHint {
    simulation::FF artifact_hash;
    simulation::FF private_function_root;
    simulation::FF public_bytecode_commitment;
    std::vector<uint8_t> packed_bytecode;

    MSGPACK_FIELDS(artifact_hash, private_function_root, public_bytecode_commitment, packed_bytecode);
};

// Temporary.
struct ExecutionHints {
    std::vector<ContractInstanceHint> contract_instances;
    std::vector<ContractClassHint> contract_classes;

    MSGPACK_FIELDS(contract_instances, contract_classes);
};

} // namespace bb::avm::simulation