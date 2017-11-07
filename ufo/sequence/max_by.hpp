#ifndef ufo_sequence_max
#define ufo_sequence_max

#include <tuple>
#include "map.hpp"
#include "find_best.hpp"
#include "../type_traits.hpp"

namespace ufo {
    template <typename F>
    constexpr auto max_by(F &&f) {
        return sequence_operator([](auto &&f, auto &&sequence) {
            using element_option = sequence_option_t<decltype(sequence)>;
            using key_option = option<decltype(f(*sequence.next()))>;
            return (std::forward<decltype(sequence)>(sequence) | map([&f](auto &&value) constexpr {
                return std::make_tuple(element_option(std::forward<decltype(value)>(value)), key_option(f(value)));
            }) | find_best([](auto &x, auto &y) constexpr {
                return *std::get<1>(x) > *std::get<1>(y);
            })).map([](auto &&tuple) constexpr -> decltype(auto) {
                return *std::get<0>(std::forward<decltype(tuple)>(tuple));
            });
        }, std::forward<F>(f));
    }
}

#endif
