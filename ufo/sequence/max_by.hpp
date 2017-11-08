#ifndef ufo_sequence_max_by
#define ufo_sequence_max_by

#include <functional>
#include "find_best_by.hpp"

namespace ufo {
    template <typename F>
    constexpr auto max_by(F &&f) {
        return find_best_by(std::forward<F>(f), std::greater<> {});
    }
}

#endif
