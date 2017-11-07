#ifndef ufo_sequence_filter
#define ufo_sequence_filter

#include "sequence_operator.hpp"
#include "sequence_wrapper.hpp"

namespace ufo {
    template <typename F>
    constexpr auto filter(F &&f) noexcept {
        return sequence_operator([](auto f, auto &&sequence) constexpr {
            return sequence_wrapper([f = std::move(f)](auto &sequence) constexpr {
                while (true) {
                    auto value = sequence.next();
                    if (!value || f(*value)) return value;
                }
            }, std::forward<decltype(sequence)>(sequence));
        }, std::forward<F>(f));
    }
}

#endif

