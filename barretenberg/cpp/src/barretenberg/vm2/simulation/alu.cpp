#include "barretenberg/vm2/simulation/alu.hpp"

#include <cstdint>
#include <memory>

namespace bb::avm::simulation {

void Alu::add(uint32_t a_addr, uint32_t b_addr, uint32_t dst_addr)
{
    auto a = memory.get(a_addr);
    auto b = memory.get(b_addr);
    memory.set(dst_addr, a + b);

    events.emit({ .operation = AluOperation::ADD,
                  .a_addr = a_addr,
                  .b_addr = b_addr,
                  .dst_addr = dst_addr,
                  .a = a,
                  .b = b,
                  .res = a + b });
}

} // namespace bb::avm::simulation