#ifndef ufo_range2_foreach
#define ufo_range2_foreach

#include "core.hpp"
#include "range_operator.hpp"

namespace ufo {
    template <typename F>
    constexpr auto foreach(F f) {
        return range_operator([f](auto &&range) constexpr {
            for (auto current = std::forward<decltype(range)>(range); !(current | is_empty); current = std::move(current) | rest) {
                f(current | first);
            }
        });
    }
}

#endif
