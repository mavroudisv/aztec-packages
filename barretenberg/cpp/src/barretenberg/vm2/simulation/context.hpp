#pragma once

#include <cstdint>
#include <memory>

#include "barretenberg/vm2/common/field.hpp"
#include "barretenberg/vm2/simulation/events/event_emitter.hpp"
#include "barretenberg/vm2/simulation/events/memory_event.hpp"
#include "barretenberg/vm2/simulation/memory.hpp"

namespace bb::avm::simulation {

class ContextInterface {
  public:
    virtual ~ContextInterface() = default;
    virtual MemoryInterface& get_memory() = 0;
    virtual uint32_t get_pc() const = 0;
    virtual void set_pc(uint32_t new_pc) = 0;
    virtual uint32_t get_next_pc() const = 0;
    virtual void set_next_pc(uint32_t new_next_pc) = 0;
    virtual void set_nested_returndata(std::vector<FF> return_data) = 0;
};

class Context : public ContextInterface {
  public:
    Context(std::unique_ptr<MemoryInterface> memory)
        : memory(std::move(memory))
    {}

    // Having getters and setters make it easier to mock the context.
    // Machine state.
    MemoryInterface& get_memory() override { return *memory; }
    uint32_t get_pc() const override { return pc; }
    void set_pc(uint32_t new_pc) override { pc = new_pc; }
    uint32_t get_next_pc() const override { return next_pc; }
    void set_next_pc(uint32_t new_next_pc) override { next_pc = new_next_pc; }
    void set_nested_returndata(std::vector<FF> return_data) override { nested_returndata = std::move(return_data); }

  private:
    uint32_t pc = 0;
    uint32_t next_pc = 0;
    std::vector<FF> nested_returndata;
    std::unique_ptr<MemoryInterface> memory;
};

class ContextProviderInterface {
  public:
    virtual ~ContextProviderInterface() = default;
    virtual std::unique_ptr<ContextInterface> make(int contract_address, uint32_t call_id) const = 0;
};

// This is the real thing. If you need a context made out of other objects, use a mock.
class ContextProvider : public ContextProviderInterface {
  public:
    ContextProvider(EventEmitterInterface<MemoryEvent>& memory_events)
        : memory_events(memory_events)
    {}
    std::unique_ptr<ContextInterface> make([[maybe_unused]] int contract_address, uint32_t call_id) const override
    {
        return std::make_unique<Context>(std::make_unique<Memory>(call_id, memory_events));
    }

  private:
    EventEmitterInterface<MemoryEvent>& memory_events;
};

} // namespace bb::avm::simulation