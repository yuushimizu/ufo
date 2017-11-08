#ifndef ufo_sequence_max
#define ufo_sequence_max

#include <functional>
#include "find_best.hpp"

namespace ufo {
    constexpr const auto max = find_best(std::greater<> {});
}

#endif
