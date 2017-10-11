#ifndef ufo_sequence_map_indexed
#define ufo_sequence_map_indexed

#include "sequence_operator.hpp"
#include "map.hpp"
#include "counting.hpp"

namespace ufo {
    template <typename F>
    constexpr auto map_indexed(F &&f) {
        return sequence_operator([](auto &&f, auto &&sequence) {
            return map(std::forward<decltype(f)>(f), counting<int>(), std::forward<decltype(sequence)>(sequence));
        }, std::forward<F>(f));
    }
}

#endif
