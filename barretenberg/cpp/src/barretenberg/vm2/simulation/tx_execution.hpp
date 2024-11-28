#pragma once

#include "barretenberg/vm2/simulation/execution.hpp"

namespace bb::avm::simulation {

// FIXME: temporarily here.
struct PublicExecutionRequest {
    AztecAddress contract_address;
    AztecAddress sender;
    std::vector<FF> args;
    bool is_static;
};

// Temporary.
struct Tx {
    std::vector<PublicExecutionRequest> enqueued_calls;
};

// In charge of executing a transaction.
class TransactionExecution final {
  public:
    TransactionExecution(ExecutionInterface& call_execution)
        : call_execution(call_execution){};

    void simulate(const Tx& tx);

  private:
    ExecutionInterface& call_execution;
};

} // namespace bb::avm::simulation