#ifndef ufo_sequence_first
#define ufo_sequence_first

#include "sequence_operator.hpp"

namespace ufo {
    constexpr auto first() noexcept {
        return sequence_operator([](auto &&sequence) {
            return std::forward<decltype(sequence)>(sequence).next();
        });
    }
}

#endif
