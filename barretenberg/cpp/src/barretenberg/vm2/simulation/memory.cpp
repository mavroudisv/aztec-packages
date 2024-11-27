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

ValueRefAndTag Memory::get(MemoryAddress index) const
{
    const auto& vt = memory.at(index);
    events.emit({ .mode = MemoryMode::READ, .addr = index, .value = vt.value, .tag = vt.tag, .space_id = space_id });
    return { vt.value, vt.tag };
}

std::pair<std::vector<MemoryValue>, std::vector<MemoryTag>> Memory::get_slice(MemoryAddress start, size_t size) const
{
    std::vector<MemoryValue> values(size);
    std::vector<MemoryTag> tags(size);
    for (size_t i = 0; i < size; ++i) {
        auto vt = get(start + i);
        values[i] = vt.value;
        tags[i] = vt.tag;
    }
    return { std::move(values), std::move(tags) };
}

} // namespace bb::avm::simulation