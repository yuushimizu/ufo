#ifndef ufo_range2_core
#define ufo_range2_core

#include "range_operator.hpp"

namespace ufo {
    constexpr auto first = range_operator([](auto &&range) constexpr -> decltype(auto) {
        return std::forward<decltype(range)>(range).first();
    });
}

#endif
