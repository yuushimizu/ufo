#ifndef ufo_sequence2_first
#define ufo_sequence2_first

#include "sequence_operator.hpp"

namespace ufo::s2 {
    constexpr auto first() noexcept {
        return sequence_operator([](auto &&sequence) {
            return sequence.begin().next();
        });
    }
}

#endif
