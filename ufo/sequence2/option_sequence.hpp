#ifndef ufo_sequence2_option_sequence
#define ufo_sequence2_option_sequence

#include "sequence.hpp"
#include "../option.hpp"

namespace ufo::s2 {
    template <typename T>
    constexpr auto option_sequence(option<T> option) noexcept {
        return sequence([option = std::move(option)] {
            return [&option, first = true]() mutable {
                if (first) {
                    first = false;
                    return option;
                } else {
                    return decltype(option) {};
                }
            };
        });
    }
}

#endif
