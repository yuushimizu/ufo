#ifndef ufo_sequence_first
#define ufo_sequence_first

#include "sequence_operator.hpp"

namespace ufo {
    constexpr inline const auto first = sequence_operator([](auto &&sequence) {
        return std::forward<decltype(sequence)>(sequence).next();
    });
}

#endif
