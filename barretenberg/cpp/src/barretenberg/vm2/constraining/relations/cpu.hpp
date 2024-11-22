#pragma once

#include "barretenberg/relations/relation_parameters.hpp"
#include "barretenberg/relations/relation_types.hpp"

namespace bb::avm::constraining {

template <typename FF_> class cpuImpl {
  public:
    using FF = FF_;

    static constexpr std::array<size_t, 2> SUBRELATION_PARTIAL_LENGTHS = { 0 };

    template <typename ContainerOverSubrelations, typename AllEntities>
    void static accumulate(ContainerOverSubrelations& evals,
                           const AllEntities& t,
                           [[maybe_unused]] const RelationParameters<FF>&,
                           const FF& scaling_factor)
    {
        // (1 - last) * (pc' - pc - 1) = 0
        {
            using Accumulator = typename std::tuple_element_t<0, ContainerOverSubrelations>;
            auto tmp = (FF(1) - t.last) * (t.pc_shifted - t.pc - FF(1));
            tmp *= scaling_factor;
            std::get<0>(evals) += typename Accumulator::View(tmp);
        }
    }
};

template <typename FF> class cpu : public Relation<cpuImpl<FF>> {
  public:
    static constexpr const char* NAME = "cpu";

    static std::string get_subrelation_label(size_t index) { return std::to_string(index); }
};

} // namespace bb::avm::constraining