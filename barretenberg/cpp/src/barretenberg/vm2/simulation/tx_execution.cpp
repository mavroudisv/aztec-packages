#include "barretenberg/vm2/simulation/tx_execution.hpp"

namespace bb::avm::simulation {

void TransactionExecution::simulate(const Tx& tx)
{
    // TODO: other inter-enqueued-call stuff will be done here.
    for (const auto& call : tx.enqueued_calls) {
        call_execution.execute(call.contract_address, call.args, call.sender, call.is_static);
        info("Enqueued call to ",
             call.contract_address,
             " returned ",
             call_execution.get_top_level_returndata().size(),
             " elements.");
    }
}

} // namespace bb::avm::simulation