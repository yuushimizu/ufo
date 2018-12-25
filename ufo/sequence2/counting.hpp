#ifndef ufo_sequence_counting
#define ufo_sequence_counting

#include "iterate.hpp"
#include "../placeholder.hpp"

namespace ufo::s2 {
    template <typename Begin, typename Step>
    constexpr auto counting(Begin begin, Step step) noexcept {
        return iterate(_ + std::move(step), std::move(begin));
    }
    
    template <typename Begin>
    constexpr auto counting(Begin begin) noexcept {
        return counting(std::move(begin), Begin(1));
    }
}

#endif
