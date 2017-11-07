#ifndef ufo_sequence_take
#define ufo_sequence_take

#include "sequence_wrapper.hpp"

namespace ufo {
    constexpr auto take(int n) noexcept {
        return sequence_wrapper([n](auto &&sequence) constexpr mutable -> sequence_option_t<decltype(sequence)> {
            if (n <= 0) return nullopt;
            if (auto value = sequence.next()) {
                --n;
                return value;
            } else {
                return nullopt;
            }
        });
    }
}

#endif
