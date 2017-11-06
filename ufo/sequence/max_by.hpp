#ifndef ufo_sequence_max
#define ufo_sequence_max

#include "sequence_operator.hpp"
#include "reduce.hpp"
#include "sequence.hpp"
#include "../option.hpp"

namespace ufo {
    template <typename F>
    constexpr auto max_by(F &&f) {
        return sequence_operator([](auto &&f, auto &&sequence) {
            using element_option = sequence_option_t<decltype(sequence)>;
            using key_option = option<decltype(f(*sequence.next()))>;
            using option_tuple = std::tuple<element_option, key_option>;
            return std::get<0>(std::forward<decltype(sequence)>(sequence) | reduce([f = std::forward<decltype(f)>(f)](auto &&result, auto &&value) -> option_tuple {
                auto &&key = f(value);
                if (!std::get<0>(result)) return std::make_tuple(element_option(std::forward<decltype(value)>(value)), key_option(std::forward<decltype(key)>(key)));
                if (key > *std::get<1>(result)) return std::make_tuple(element_option(std::forward<decltype(value)>(value)), key_option(std::forward<decltype(key)>(key)));
                return std::move(result);
            }, option_tuple {}));
        }, std::forward<F>(f));
    }
}

#endif
