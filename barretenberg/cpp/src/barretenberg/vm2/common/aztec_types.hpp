#pragma once

#include <cstdint>
#include <vector>

#include "barretenberg/vm2/common/field.hpp"

namespace bb::avm {

using AztecAddress = simulation::FF;
using ContractClassId = simulation::FF;
using AffinePoint = grumpkin::g1::affine_element;

struct PublicKeys {
    AffinePoint nullifier_key;
    /** Incoming viewing public key */
    AffinePoint incoming_viewing_key;
    /** Outgoing viewing public key */
    AffinePoint outgoing_viewing_key;
    /** Tagging viewing public key */
    AffinePoint tagging_key;
};

struct ContractInstance {
    AztecAddress address;
    simulation::FF salt;
    AztecAddress deployer_addr;
    ContractClassId contract_class_id;
    simulation::FF initialisation_hash;
    PublicKeys public_keys;
};

struct ContractClass {
    simulation::FF artifact_hash;
    simulation::FF private_function_root;
    simulation::FF public_bytecode_commitment;
    std::vector<uint8_t> packed_bytecode;
};

} // namespace bb::avm