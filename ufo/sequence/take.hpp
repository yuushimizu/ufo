#ifndef ufo_sequence_take
#define ufo_sequence_take

#include "sequence_operator.hpp"
#include "sequence_wrapper.hpp"

namespace ufo {
    constexpr auto take(int n) noexcept {
        return sequence_operator([n](auto &&sequence) constexpr {
            return sequence_wrapper([n](auto &sequence) constexpr mutable -> sequence_option_t<decltype(sequence)> {
                if (n <= 0) return nullopt;
                if (auto value = sequence.next()) {
                    --n;
                    return value;
                } else {
                    return nullopt;
                }
            }, std::forward<decltype(sequence)>(sequence));
        });
    }
}

#endif
