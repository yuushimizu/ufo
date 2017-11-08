#ifndef ufo_sequence_min
#define ufo_sequence_min

#include <functional>
#include "find_best.hpp"

namespace ufo {
    constexpr const auto min = find_best(std::less<> {});
}

#endif
