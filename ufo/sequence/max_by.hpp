#ifndef ufo_sequence_max_by
#define ufo_sequence_max_by

#include "find_best_by.hpp"

namespace ufo {
    template <typename F>
    constexpr auto max_by(F &&f) {
        return find_best_by(std::forward<F>(f), [](auto &&x, auto &&y) {
            return std::forward<decltype(x)>(x) > std::forward<decltype(y)>(y);
        });
    }
}

#endif
