#ifndef ufo_sequence_take_while
#define ufo_sequence_take_while

#include "sequence_operator.hpp"
#include "sequence.hpp"
#include "../functional.hpp"
#include "../box.hpp"

namespace ufo::s2 {
    template <typename F>
    constexpr auto take_while(F f) noexcept {
        return sequence_operator(partial([](auto &&f, auto &&seq) {
            return sequence([f = function_wrapper(std::forward<decltype(f)>(f)), seq = forward_box<decltype(seq)>(seq)]() {
                return [&f, base = seq->begin()]() mutable {
                    auto next = base.next();
                    return next && f(*next) ? next : decltype(next) {};
                };
            });
        }, std::move(f)));
    }
}

#endif
