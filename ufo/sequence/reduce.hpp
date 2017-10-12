#ifndef ufo_sequence_reduce
#define ufo_sequence_reduce

#include "sequence_operator.hpp"
#include "foreach.hpp"
#include "../option.hpp"

namespace ufo {
    template <typename F, typename T>
    constexpr auto reduce(F &&f, T &&initial_value) {
        return sequence_operator([](auto &&f, auto &&initial_value, auto &&sequence) -> T {
            option<T> result = std::forward<decltype(initial_value)>(initial_value);
            std::forward<decltype(sequence)>(sequence) | foreach([&f, &result](auto &&value) {
                result = f(*std::move(result), std::forward<decltype(value)>(value));
            });
            return *std::move(result);
        }, std::forward<F>(f), std::forward<T>(initial_value));
    }
}

#endif
