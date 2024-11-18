#pragma once

#include <cstddef>
#include <cstdint>
#include <ranges>

#include "barretenberg/vm2/generated/columns.hpp"
#include "barretenberg/vm2/simulation/events/alu_event.hpp"
#include "barretenberg/vm2/tracegen/trace_container.hpp"

namespace bb::avm::tracegen {

class AluTraceBuilder final {
  public:
    // TODO: do not take a list here. C++ doesnt just let me take any iterable.
    void process(const std::list<simulation::AluEvent>& events, TraceContainer& trace)
    {
        using C = Column;

        size_t row = 0;
        for (const auto& event : events) {
            trace.set(C::alu_opcode, row, static_cast<uint8_t>(event.opcode));
            trace.set(C::alu_a, row, event.a);
            trace.set(C::alu_b, row, event.b);
            trace.set(C::alu_c, row, event.res);
            trace.set(C::alu_a_addr, row, event.a_addr);
            trace.set(C::alu_b_addr, row, event.b_addr);
            trace.set(C::alu_dst_addr, row, event.dst_addr);

            row++;
        }
    }
};

} // namespace bb::avm::tracegen