#ifndef ufo_sequence2_empty
#define ufo_sequence2_empty

#include "sequence.hpp"
#include "option.hpp"

namespace ufo::s2 {
    template <typename T>
    constexpr auto empty() noexcept {
        return sequence([] {
            return [] {
                return option<T> {};
            };
        });
    }
}

#endif
