#ifndef ufo_sequence_nth
#define ufo_sequence_nth

#include "sequence_operator.hpp"
#include "skip.hpp"
#include "first.hpp"

namespace ufo {
    constexpr auto nth(int n) {
        return skip(n) | first();
    }
}

#endif
