#ifndef ufo_sequence_foreach
#define ufo_sequence_foreach

#include <functional>
#include "sequence_operator.hpp"

namespace ufo {
    template <typename F>
    constexpr auto foreach(F f) {
        return sequence_operator([f = std::move(f)](auto &&sequence) constexpr {
            auto s = std::forward<decltype(sequence)>(sequence);
            while (auto value = s.next()) std::invoke(f, *value);
        });
    }
}

#endif
