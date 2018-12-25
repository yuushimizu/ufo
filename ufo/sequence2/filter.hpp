#ifndef ufo_sequence2_filter
#define ufo_sequence2_filter

#include "sequence_operator.hpp"
#include "sequence.hpp"
#include "../functional.hpp"

namespace ufo::s2 {
    template <typename F>
    constexpr auto filter(F f) noexcept {
        return sequence_operator(partial([](auto &&f, auto &&seq) {
            return sequence([f = function_wrapper(std::forward<decltype(f)>(f)), seq = forward_box<decltype(seq)>(seq)] {
                return [&f, base = seq->begin()]() mutable {
                    while (true) {
                        auto next = base.next();
                        if (!next || f(*next)) return next;
                    }
                };
            });
        }, std::move(f)));
    }
}

#endif
