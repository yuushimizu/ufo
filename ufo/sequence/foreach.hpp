#ifndef ufo_sequence_foreach
#define ufo_sequence_foreach

#include "sequence_operator.hpp"

namespace ufo {
    template <typename F>
    constexpr auto foreach(F f) {
        return sequence_operator([f = std::move(f)](auto &&sequence) constexpr {
            for (auto current = std::forward<decltype(sequence)>(sequence); !current.empty(); current.pop()) {
                f(current.front());
            }
        });
    }
}

#endif
