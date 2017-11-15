#ifndef ufo_sequence_max
#define ufo_sequence_max

#include <functional>
#include "find_best.hpp"

namespace ufo {
    constexpr auto max() noexcept {
        return find_best(std::greater<> {});
    }
}

#endif
