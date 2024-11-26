#pragma once

#include <cstdint>
#include <memory>

#include "barretenberg/vm2/simulation/events/event_emitter.hpp"
#include "barretenberg/vm2/simulation/events/memory_event.hpp"
#include "barretenberg/vm2/simulation/memory.hpp"

namespace bb::avm::simulation {

class Context final {
  public:
    Context(std::unique_ptr<MemoryInterface> memory)
        : memory(std::move(memory))
    {}

    // Machine state.
    MemoryInterface& get_memory() { return *memory; }
    uint32_t get_pc() const { return pc; }
    void set_pc(uint32_t new_pc) { pc = new_pc; }
    uint32_t get_next_pc() const { return next_pc; }
    void set_next_pc(uint32_t new_next_pc) { next_pc = new_next_pc; }

  private:
    uint32_t pc = 0;
    uint32_t next_pc = 0;
    std::unique_ptr<MemoryInterface> memory;
};

class ContextProviderInterface {
  public:
    virtual ~ContextProviderInterface() = default;
    virtual std::unique_ptr<Context> make(int contract_address, uint32_t call_id) const = 0;
};

// This is the real thing. If you need a context made out of other objects, use a mock.
class ContextProvider : public ContextProviderInterface {
  public:
    ContextProvider(EventEmitterInterface<MemoryEvent>& memory_events)
        : memory_events(memory_events)
    {}
    std::unique_ptr<Context> make(int contract_address, [[maybe_unused]] uint32_t call_id) const override
    {
        return std::make_unique<Context>(std::make_unique<Memory>(contract_address, memory_events));
    }

  private:
    EventEmitterInterface<MemoryEvent>& memory_events;
};

} // namespace bb::avm::simulation