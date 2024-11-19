#pragma once

#include <cstddef>
#include <cstdint>
#include <ranges>
#include <stdexcept>

#include "barretenberg/vm2/generated/columns.hpp"
#include "barretenberg/vm2/simulation/events/alu_event.hpp"
#include "barretenberg/vm2/simulation/opcodes.hpp"
#include "barretenberg/vm2/tracegen/trace_container.hpp"

namespace bb::avm::tracegen {

using simulation::OpCode;

namespace {

Column get_opcode_selector(simulation::OpCode opcode)
{
    switch (opcode) {
    case OpCode::ADD:
        return Column::alu_sel_op_add;
    default:
        throw std::runtime_error("Unknown ALU opcode");
        break;
    }
}

} // namespace

class AluTraceBuilder final {
  public:
    // TODO: do not take a list here. C++ doesnt just let me take any iterable.
    void process(const std::list<simulation::AluEvent>& events, TraceContainer& trace)
    {
        using C = Column;

        size_t row = 0;
        for (const auto& event : events) {
            C opcode_selector = get_opcode_selector(event.opcode);

            trace.set(row,
                      { {
                          { opcode_selector, 1 },
                          { C::alu_opcode, static_cast<uint8_t>(event.opcode) },
                          { C::alu_a, event.a },
                          { C::alu_b, event.b },
                          { C::alu_c, event.res },
                          { C::alu_a_addr, event.a_addr },
                          { C::alu_b_addr, event.b_addr },
                          { C::alu_dst_addr, event.dst_addr },
                      } });

            row++;
        }
    }
};

} // namespace bb::avm::tracegen