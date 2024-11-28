#include "barretenberg/vm2/simulation/bytecode_manager.hpp"

#include "barretenberg/vm2/simulation/serialization.hpp"

namespace bb::avm::simulation {

std::pair<Instruction, /*read_bytes*/ uint32_t> BytecodeManager::read_instruction(size_t pc) const
{
    return decode_instruction(bytecode, pc);
}

} // namespace bb::avm::simulation