#ifndef ufo_range2_core
#define ufo_range2_core

#include "range_operator.hpp"

namespace ufo {
    constexpr auto first = range_operator([](auto &&range) constexpr -> decltype(auto) {
        return std::forward<decltype(range)>(range).first();
    });
    
    constexpr auto rest = range_operator([](auto &&range) constexpr -> decltype(auto) {
        auto result = std::forward<decltype(range)>(range);
        result.pop();
        return result;
    });
    
    constexpr auto is_empty = range_operator([](const auto &range) constexpr -> bool {
        return range.empty();
    });
}

#endif
