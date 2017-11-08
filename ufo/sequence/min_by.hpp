#ifndef ufo_sequence_min_by
#define ufo_sequence_min_by

#include <functional>
#include "find_best_by.hpp"

namespace ufo {
    template <typename F>
    constexpr auto min_by(F &&f) {
        return find_best_by(std::forward<F>(f), std::less<> {});
    }
}

#endif

