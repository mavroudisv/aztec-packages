#include "barretenberg/vm2/simulation/memory.hpp"

#include <memory>
#include <unordered_map>

namespace bb::avm::simulation {

void Memory::set(MemoryAddress index, MemoryValue value)
{
    memory[index] = value;
    events.emit({ .mode = MemoryMode::WRITE, .addr = index, .value = value, .space_id = space_id });
}

MemoryValue Memory::get(MemoryAddress index) const
{
    auto value = memory.at(index);
    events.emit({ .mode = MemoryMode::READ, .addr = index, .value = value, .space_id = space_id });
    return value;
}

} // namespace bb::avm::simulation