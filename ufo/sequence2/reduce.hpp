#ifndef ufo_sequence2_reduce
#define ufo_sequence2_reduce

#include "sequence_operator.hpp"
#include "foreach.hpp"
#include "../functional.hpp"

namespace ufo::s2 {
    template <typename V, typename F>
    constexpr decltype(auto) reduce(V initial_value, F f) {
        return sequence_operator(partial([f = std::move(f)](auto &&initial_value, auto &&seq) {
            V result = std::forward<decltype(initial_value)>(initial_value);
            std::forward<decltype(seq)>(seq) | foreach([&result, &f](auto &&value) {
                V next = std::invoke(f, std::move(result), std::forward<decltype(value)>(value));
                result = std::move(next);
            });
            return result;
        }, std::move(initial_value)));
    }
}

#endif
