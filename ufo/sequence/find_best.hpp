#ifndef ufo_sequence_find_best
#define ufo_sequence_find_best

#include "sequence_operator.hpp"
#include "reduce.hpp"
#include "sequence.hpp"

namespace ufo {
    template <typename F>
    constexpr auto find_best(F &&f) {
        return sequence_operator([](auto &&f, auto &&sequence) {
            using element_option = sequence_option_t<decltype(sequence)>;
            return std::forward<decltype(sequence)>(sequence) | reduce([&f](auto &&result, auto &&value) {
                if (!result || f(value, *result)) return element_option(std::forward<decltype(value)>(value));
                return std::forward<decltype(result)>(result);
            }, element_option {});
        }, std::forward<F>(f));
    }
}

#endif
