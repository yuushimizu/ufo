#ifndef ufo_sequence_find
#define ufo_sequence_find

#include "sequence_operator.hpp"
#include "filter.hpp"
#include "first.hpp"

namespace ufo {
    template <typename F>
    constexpr auto find(F &&f) {
        return filter(std::forward<F>(f)) | first;
    }
}

#endif
