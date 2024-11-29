#include "barretenberg/vm2/common/instruction_spec.hpp"

#include "barretenberg/vm/aztec_constants.hpp" // Move over.
#include "barretenberg/vm2/common/opcodes.hpp"

namespace bb::avm {

const std::unordered_map<ExecutionOpCode, InstructionSpec> INSTRUCTION_SPEC = {
    { ExecutionOpCode::ADD,
      { .num_addresses = 3, .gas_cost = { .base_l2 = AVM_ADD_BASE_L2_GAS, .base_da = 0, .dyn_l2 = 0, .dyn_da = 0 } } },
    { ExecutionOpCode::CALL,
      { .num_addresses = 6,
        .gas_cost = { .base_l2 = AVM_CALL_BASE_L2_GAS, .base_da = 0, .dyn_l2 = AVM_CALL_DYN_L2_GAS, .dyn_da = 0 } } },
    { ExecutionOpCode::RETURN,
      { .num_addresses = 2,
        .gas_cost = { .base_l2 = AVM_RETURN_BASE_L2_GAS,
                      .base_da = 0,
                      .dyn_l2 = AVM_RETURN_DYN_L2_GAS,
                      .dyn_da = 0 } } },
    { ExecutionOpCode::JUMPI,
      { .num_addresses = 1,
        .gas_cost = { .base_l2 = AVM_JUMPI_BASE_L2_GAS, .base_da = 0, .dyn_l2 = 0, .dyn_da = 0 } } },
};

} // namespace bb::avm