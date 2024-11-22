#include "barretenberg/vm2/simulation/memory.hpp"

#include <memory>
#include <unordered_map>

namespace bb::avm::simulation {

void Memory::set(size_t index, int value)
{
    memory[index] = value;
    events.emit(MemoryEvent{ .mode = MemoryMode::WRITE, .addr = index, .value = value });
}
int Memory::get(size_t index) const
{
    auto value = memory.at(index);
    events.emit(MemoryEvent{ .mode = MemoryMode::READ, .addr = index, .value = value });
    return value;
}

} // namespace bb::avm::simulation