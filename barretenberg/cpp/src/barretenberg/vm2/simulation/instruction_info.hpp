#pragma once

#include <cstdint>
#include <memory>

#include "barretenberg/vm2/common/instruction_spec.hpp"
#include "barretenberg/vm2/common/opcodes.hpp"
#include "barretenberg/vm2/simulation/events/event_emitter.hpp"
#include "barretenberg/vm2/simulation/events/instruction_info_event.hpp"

namespace bb::avm::simulation {

class InstructionInformationInterface {
  public:
    virtual ~InstructionInformationInterface() = default;

    virtual InstructionSpec get(ExecutionOpCode opcode) = 0;
};

class InstructionInformation : public InstructionInformationInterface {
  public:
    InstructionInformation(EventEmitterInterface<InstructionInformationEvent>& event_emitter)
        : events(event_emitter)
    {}

    InstructionSpec get(ExecutionOpCode opcode) override
    {
        // FIXME: I think we don't need events here!
        // This will be a fixed table, and we can handle the lookups in tracegen.
        events.emit({ .opcode = opcode });
        return INSTRUCTION_SPEC.at(opcode);
    }

  private:
    EventEmitterInterface<InstructionInformationEvent>& events;
};

} // namespace bb::avm::simulation