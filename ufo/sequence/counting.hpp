#ifndef ufo_sequence_couting
#define ufo_sequence_couting

#include "iterate.hpp"
#include "../placeholder.hpp"

namespace ufo {
    template <typename T, typename Step>
    constexpr auto couting(T begin, Step step) {
        return iterate(_ + std::move(step), std::move(begin));
    }
    
    template <typename T>
    constexpr auto couting(T begin) {
        return couting(std::move(begin), T(1));
    }
    
    template <typename T>
    constexpr auto couting() {
        return couting(T {});
    }
}

#endif
