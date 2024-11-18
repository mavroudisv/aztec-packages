#pragma once

#include <cstdint>
#include <memory>

#include "barretenberg/vm2/simulation/events/alu_event.hpp"
#include "barretenberg/vm2/simulation/events/event_emitter.hpp"
#include "barretenberg/vm2/simulation/memory.hpp"

namespace bb::avm::simulation {

class Alu final {
  public:
    Alu(Memory& mem, EventEmitterInterface<AluEvent>& event_emitter)
        : memory(mem)
        , events(event_emitter)
    {}

    // Operands are expected to be direct.
    void add(uint32_t a_addr, uint32_t b_addr, uint32_t dst_addr)
    {
        auto a = memory.get(a_addr);
        auto b = memory.get(b_addr);
        memory.set(dst_addr, a + b);

        events.emit(AluEvent{ .opcode = OpCode::ADD,
                              .a_addr = a_addr,
                              .b_addr = b_addr,
                              .dst_addr = dst_addr,
                              .a = a,
                              .b = b,
                              .res = a + b });
    }

  private:
    Memory& memory;
    EventEmitterInterface<AluEvent>& events;
};

} // namespace bb::avm::simulation