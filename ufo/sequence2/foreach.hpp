#ifndef ufo_sequence_foreach
#define ufo_sequence_foreach

#include "sequence_operator.hpp"

namespace ufo::s2 {
    template <typename F>
    constexpr auto foreach(F f) noexcept {
        return sequence_operator([f = std::move(f)](auto &&seq) {
            auto i = seq.begin();
            while (auto &&element = i.next()) std::invoke(f, *std::forward<decltype(element)>(element));
        });
    }
}

#endif
