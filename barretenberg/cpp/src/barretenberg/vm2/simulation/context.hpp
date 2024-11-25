#pragma once

#include <cstdint>
#include <memory>

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

  private:
    uint32_t pc = 0;
    std::unique_ptr<MemoryInterface> memory;
};

class ContextProviderInterface {
  public:
    virtual ~ContextProviderInterface() = default;
    virtual std::unique_ptr<Context> make(std::unique_ptr<MemoryInterface> memory) const = 0;
};

class ContextProvider : public ContextProviderInterface {
  public:
    virtual ~ContextProvider() = default;
    virtual std::unique_ptr<Context> make(std::unique_ptr<MemoryInterface> memory) const
    {
        return std::make_unique<Context>(std::move(memory));
    }
};

} // namespace bb::avm::simulation