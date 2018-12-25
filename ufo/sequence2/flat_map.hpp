#ifndef ufo_sequence2_flat_map
#define ufo_sequence2_flat_map

#include "map.hpp"
#include "flatten.hpp"

namespace ufo::s2 {
    template <typename F>
    constexpr auto flat_map(F f) noexcept {
        return map(std::move(f)) | flatten();
    }
}


#endif
