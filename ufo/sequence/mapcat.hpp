#ifndef ufo_sequence_mapcat
#define ufo_sequence_mapcat

#include "sequence_operator.hpp"
#include "map.hpp"
#include "flatten.hpp"

namespace ufo {
    template <typename F>
    constexpr auto mapcat(F &&f) {
        return map(std::forward<F>(f)) | flatten();
    }
}

#endif
