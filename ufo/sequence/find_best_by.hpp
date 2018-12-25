#ifndef ufo_sequence_find_best_by
#define ufo_sequence_find_best_by

#include <tuple>
#include <functional>
#include "map.hpp"
#include "find_best.hpp"
#include "../type_traits.hpp"
#include "../box.hpp"

namespace ufo {
    template <typename KeyF, typename CompareF>
    constexpr auto find_best_by(KeyF &&key_f, CompareF &&compare_f) {
        return sequence_operator([](auto &&key_f, auto &&compare_f, auto &&sequence) {
            using element_holder = box<sequence_element_t<decltype(sequence)>>;
            using key_holder = box<decltype(std::invoke(key_f, *sequence.next()))>;
            return (std::forward<decltype(sequence)>(sequence) | map([&key_f](auto &&value) constexpr {
                auto key = key_holder(std::invoke(key_f, value));
                return std::make_tuple(element_holder(std::forward<decltype(value)>(value)), std::move(key));
            }) | find_best([&compare_f](const auto &x, const auto &y) constexpr {
                return compare_f(std::get<1>(x).get(), std::get<1>(y).get());
            })).map([](auto &&tuple) constexpr -> decltype(auto) {
                return std::get<0>(std::forward<decltype(tuple)>(tuple)).get();
            });
        }, std::forward<KeyF>(key_f), std::forward<CompareF>(compare_f));
    }
}

#endif
