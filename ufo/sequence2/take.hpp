#ifndef ufo_sequence2_take
#define ufo_sequence2_take

#include <cstddef>
#include "sequence_operator.hpp"
#include "sequence.hpp"

namespace ufo::s2 {
    constexpr auto take(std::size_t count) noexcept {
        return sequence_operator([count](auto &&seq) {
            return sequence([count, seq = std::forward<decltype(seq)>(seq)]() {
                return [rest = count, base = seq.begin()]() mutable {
                    if (rest <= 0) return decltype(base.next()) {};
                    auto next = base.next();
                    --rest;
                    return next;
                };
            });
        });
    }
}

#endif
