#ifndef ufo_sequence_mapcat
#define ufo_sequence_mapcat

#include "sequence_operator.hpp"
#include "map.hpp"
#include "flatten.hpp"

namespace ufo {
    template <typename F>
    constexpr auto mapcat(F &&f) {
        return sequence_operator([](auto &&f, auto &&sequence) {
            return std::forward<decltype(sequence)>(sequence) | map(std::forward<decltype(f)>(f)) | flatten;
        }, std::forward<F>(f));
    }
}

#endif
