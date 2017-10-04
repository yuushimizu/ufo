#ifndef ufo_range2_foreach
#define ufo_range2_foreach

#include "range_operator.hpp"

namespace ufo {
    template <typename F>
    constexpr auto foreach(F f) {
        return range_operator([f = std::move(f)](auto &&range) constexpr {
            for (auto current = std::forward<decltype(range)>(range); !current.empty(); current.pop()) {
                f(current.first());
            }
        });
    }
}

#endif
