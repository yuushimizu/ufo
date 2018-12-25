#ifndef ufo_sequence_iterate
#define ufo_sequence_iterate

#include "sequence.hpp"
#include "../option.hpp"
#include "../functional.hpp"

namespace ufo::s2 {
    template <typename F, typename V>
    constexpr auto iterate(F f, V initial_value) noexcept {
        return sequence([f = function_wrapper(std::move(f)), initial_value = std::move(initial_value)]() {
            return [&f, &initial_value, current = option<V> {}]() mutable {
                if (!current) {
                    current = initial_value;
                } else {
                    current.emplace(f(std::move(*current)));
                }
                return current;
            };
        });
    }
}

#endif
