#ifndef ufo_range_arithmetic_progression
#define ufo_range_arithmetic_progression

#include <utility>
#include "iterate.hpp"

namespace ufo {
    template <typename Begin, typename Step>
    constexpr auto arithmetic_progression(Begin begin, Step step) {
        return iterate(std::move(begin), [step = std::move(step)](auto &&n) {return n + step;});
    }
    
    template <typename T>
    constexpr auto arithmetic_progression(T &&begin) {
        return arithmetic_progression(std::forward<T>(begin), 1);
    }
    
    template <typename T = int>
    constexpr auto arithmetic_progression() {
        return arithmetic_progression(T {});
    }
}

#endif
