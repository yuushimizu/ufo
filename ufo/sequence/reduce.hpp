#ifndef ufo_sequence_reduce
#define ufo_sequence_reduce

#include <functional>
#include "sequence_operator.hpp"
#include "foreach.hpp"

namespace ufo {
    template <typename T, typename F>
    constexpr auto reduce(F &&f, T &&initial_value) {
        return sequence_operator([](auto &&f, auto &&initial_value, auto &&sequence) -> T {
            T result = std::forward<decltype(initial_value)>(initial_value);
            std::forward<decltype(sequence)>(sequence) | foreach([&f, &result](auto &&value) {
                result = std::invoke(f, std::forward<T>(result), std::forward<decltype(value)>(value));
            });
            return std::forward<T>(result);
        }, std::forward<F>(f), std::forward<T>(initial_value));
    }
}

#endif
