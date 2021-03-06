#ifndef ufo_sequence_counting
#define ufo_sequence_counting

#include "iterate.hpp"
#include "../placeholder.hpp"

namespace ufo {
    template <typename Begin, typename Step>
    constexpr auto counting(Begin begin, Step step) {
        return iterate(_ + std::move(step), std::move(begin));
    }
    
    template <typename Begin>
    constexpr auto counting(Begin begin) {
        return counting(std::move(begin), Begin(1));
    }
    
    template <typename T>
    constexpr auto counting() {
        return counting(T {});
    }
}

#endif
