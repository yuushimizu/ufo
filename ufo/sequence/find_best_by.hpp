#ifndef ufo_sequence_find_best_by
#define ufo_sequence_find_best_by

#include <tuple>
#include "map.hpp"
#include "find_best.hpp"
#include "../type_traits.hpp"

namespace ufo {
    template <typename KeyF, typename CompareF>
    constexpr auto find_best_by(KeyF &&key_f, CompareF &&compare_f) {
        return sequence_operator([](auto &&key_f, auto &&compare_f, auto &&sequence) {
            using element_option = sequence_option_t<decltype(sequence)>;
            using key_option = option<decltype(key_f(*sequence.next()))>;
            return (std::forward<decltype(sequence)>(sequence) | map([&key_f](auto &&value) constexpr {
                return std::make_tuple(element_option(std::forward<decltype(value)>(value)), key_option(key_f(value)));
            }) | find_best([&compare_f](auto &x, auto &y) constexpr {
                return compare_f(*std::get<1>(x), *std::get<1>(y));
            })).map([](auto &&tuple) constexpr -> decltype(auto) {
                return *std::get<0>(std::forward<decltype(tuple)>(tuple));
            });
        }, std::forward<KeyF>(key_f), std::forward<CompareF>(compare_f));
    }
}

#endif
