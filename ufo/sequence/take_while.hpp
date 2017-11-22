#ifndef ufo_sequence_take_while
#define ufo_sequence_take_while

#include <functional>
#include "sequence_operator.hpp"
#include "sequence_wrapper.hpp"

namespace ufo {
    template <typename F>
    constexpr auto take_while(F &&f) noexcept {
        return sequence_operator([](auto f, auto &&sequence) constexpr {
            return sequence_wrapper([f = std::move(f), finished = false](auto &sequence) constexpr mutable -> sequence_option_t<decltype(sequence)> {
                if (finished) return nullopt;
                if (auto value = sequence.next()) {
                    if (std::invoke(f, *value)) return value;
                }
                finished = true;
                return nullopt;
            }, std::forward<decltype(sequence)>(sequence));
        }, std::forward<F>(f));
    }
}

#endif
