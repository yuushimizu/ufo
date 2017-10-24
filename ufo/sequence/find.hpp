#ifndef ufo_sequence_find
#define ufo_sequence_find

#include "sequence_operator.hpp"
#include "filter.hpp"
#include "first.hpp"

namespace ufo {
    template <typename F>
    constexpr auto find(F &&f) {
        return sequence_operator([](auto &&f, auto &&sequence) {
            return std::forward<decltype(sequence)>(sequence) | filter(std::forward<decltype(f)>(f)) | first;
        }, std::forward<F>(f));
    }
}

#endif
