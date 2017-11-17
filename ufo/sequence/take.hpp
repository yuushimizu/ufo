#ifndef ufo_sequence_take
#define ufo_sequence_take

#include <cstddef>
#include "sequence_operator.hpp"
#include "sequence_wrapper.hpp"

namespace ufo {
    constexpr auto take(std::size_t n) noexcept {
        return sequence_operator([n](auto &&sequence) constexpr {
            return sequence_wrapper([n](auto &sequence) constexpr mutable -> sequence_option_t<decltype(sequence)> {
                if (n <= 0) return nullopt;
                auto value = sequence.next();
                if (value) --n;
                return value;
            }, std::forward<decltype(sequence)>(sequence));
        });
    }
}

#endif
