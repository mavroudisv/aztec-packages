#pragma once

#include <array>
#include <span>
#include <stdexcept>

#include "barretenberg/common/log.hpp"

namespace bb::avm::constraining {

template <typename Relation, typename Trace> void check_relation(const Trace& trace, std::span<size_t> subrelations)
{
    typename Relation::SumcheckArrayOfValuesOverSubrelations result{};

    for (size_t r = 0; r < trace.size(); ++r) {
        Relation::accumulate(result, trace.at(r), {}, 1);
        for (size_t j : subrelations) {
            if (result[j] != 0) {
                throw std::runtime_error(format("Relation ",
                                                Relation::NAME,
                                                ", subrelation ",
                                                Relation::get_subrelation_label(j),
                                                " failed at row ",
                                                r));
            }
        }
    }
}

template <typename Relation, typename Trace> void check_relation(const Trace& trace)
{
    std::array<size_t, Relation::SUBRELATION_PARTIAL_LENGTHS.size()> subrelations{};
    for (size_t i = 0; i < subrelations.size(); ++i) {
        subrelations[i] = i;
    }
    check_relation<Relation>(trace, subrelations);
}

template <typename Relation, typename Trace> void check_relation(const Trace& trace, size_t subrelation)
{
    std::array<size_t, 1> subrelations = { subrelation };
    check_relation<Relation>(trace, subrelations);
}

} // namespace bb::avm::constraining