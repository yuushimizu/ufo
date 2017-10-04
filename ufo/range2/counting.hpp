#ifndef ufo_range2_couting
#define ufo_range2_couting

#include "iterate.hpp"

namespace ufo {
    template <typename T, typename Step>
    constexpr auto couting(T begin, Step step) {
        return iterate([step = std::move(step)](auto current) constexpr {
            return current + step;
        }, std::move(begin));
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
