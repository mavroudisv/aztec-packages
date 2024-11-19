#pragma once

#include "barretenberg/relations/relation_parameters.hpp"
#include "barretenberg/relations/relation_types.hpp"

namespace bb::avm::constraining {

template <typename FF_> class aluImpl {
  public:
    using FF = FF_;

    static constexpr std::array<size_t, 2> SUBRELATION_PARTIAL_LENGTHS = { 1, 1 };

    template <typename ContainerOverSubrelations, typename AllEntities>
    void static accumulate(ContainerOverSubrelations& evals,
                           const AllEntities& new_term,
                           [[maybe_unused]] const RelationParameters<FF>&,
                           const FF& scaling_factor)
    {
        // alu_sel_op_add * (1 - alu_sel_op_add) = 0
        {
            using Accumulator = typename std::tuple_element_t<0, ContainerOverSubrelations>;
            auto tmp = (new_term.alu_sel_op_add * (FF(1) - new_term.alu_sel_op_add));
            tmp *= scaling_factor;
            std::get<0>(evals) += typename Accumulator::View(tmp);
        }
        // a + b = c
        {
            using Accumulator = typename std::tuple_element_t<1, ContainerOverSubrelations>;
            auto tmp = (new_term.alu_a + new_term.alu_b - new_term.alu_c);
            tmp *= scaling_factor;
            std::get<1>(evals) += typename Accumulator::View(tmp);
        }
    }
};

template <typename FF> class alu : public Relation<aluImpl<FF>> {
  public:
    static constexpr const char* NAME = "alu";

    static std::string get_subrelation_label(size_t index) { return std::to_string(index); }
};

} // namespace bb::avm::constraining