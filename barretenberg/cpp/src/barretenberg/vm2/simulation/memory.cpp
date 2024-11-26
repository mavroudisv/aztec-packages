#include "barretenberg/vm2/simulation/memory.hpp"

#include <memory>
#include <unordered_map>

namespace bb::avm::simulation {

void Memory::set(MemoryAddress index, MemoryValue value, MemoryTag tag)
{
    // TODO: validate tag-value makes sense.
    memory[index] = { value, tag };
    events.emit({ .mode = MemoryMode::WRITE, .addr = index, .value = value, .tag = tag, .space_id = space_id });
}

ValueAndTag Memory::get(MemoryAddress index) const
{
    auto vt = memory.at(index);
    events.emit({ .mode = MemoryMode::READ, .addr = index, .value = vt.value, .tag = vt.tag, .space_id = space_id });
    return vt;
}

} // namespace bb::avm::simulation