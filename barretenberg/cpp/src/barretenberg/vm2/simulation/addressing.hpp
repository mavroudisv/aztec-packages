#pragma once

#include <array>

#include "barretenberg/vm2/simulation/memory.hpp"

namespace bb::avm::simulation {

// TODO: Make this class emit events.
template <size_t N> class Addressing final {
  public:
    Addressing(uint16_t indirect)
        : indirect(indirect)
    {}

    std::array<uint32_t, N> resolve(const std::array<uint32_t, N>& offsets, [[maybe_unused]] Memory& memory) const
    {
        // Doesn't do anything right now, but it could access memory.
        return offsets;
    }

  private:
    [[maybe_unused]] uint16_t indirect;
};

} // namespace bb::avm::simulation