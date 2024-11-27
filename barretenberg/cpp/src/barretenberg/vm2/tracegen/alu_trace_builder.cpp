#include "barretenberg/vm2/tracegen/alu_trace_builder.hpp"

#include <cstddef>
#include <cstdint>
#include <ranges>
#include <stdexcept>

#include "barretenberg/vm2/simulation/events/alu_event.hpp"
#include "barretenberg/vm2/simulation/events/event_emitter.hpp"

namespace bb::avm::tracegen {

namespace {

Column get_operation_selector(simulation::AluOperation operation)
{
    switch (operation) {
    case simulation::AluOperation::ADD:
        return Column::alu_sel_op_add;
    default:
        throw std::runtime_error("Unknown ALU operation");
        break;
    }
}

} // namespace

void AluTraceBuilder::process(const simulation::EventEmitterInterface<simulation::AluEvent>::Container& events,
                              TraceContainer& trace)
{
    using C = Column;

    size_t row = 0;
    for (const auto& event : events) {
        C opcode_selector = get_operation_selector(event.operation);

        trace.set(row,
                  { {
                      { opcode_selector, 1 },
                      { C::alu_operation, static_cast<uint8_t>(event.operation) },
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

} // namespace bb::avm::tracegen