#ifndef ufo_sequence_skip
#define ufo_sequence_skip

#include <cstddef>
#include "sequence_operator.hpp"
#include "sequence_wrapper.hpp"

namespace ufo {
    constexpr auto skip(std::size_t n) noexcept {
        return sequence_operator([n](auto &&sequence) constexpr {
            return sequence_wrapper([n](auto &sequence) constexpr mutable {
                while (n > 0) {
                    sequence.next();
                    --n;
                }
                return sequence.next();
            }, std::forward<decltype(sequence)>(sequence));
        });
    }
}

#endif
