#pragma once

#include <tuple>

#include "barretenberg/honk/proof_system/types/proof.hpp"

namespace bb::avm {

class AvmAPI {
  public:
    using AvmProof = bb::HonkProof;
    using AvmVerificationKey = int;

    AvmAPI() = default;

    std::tuple<AvmProof, AvmVerificationKey> prove();
};

} // namespace bb::avm