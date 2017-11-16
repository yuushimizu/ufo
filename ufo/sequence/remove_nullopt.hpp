#ifndef ufo_sequence_remove_nullopt
#define ufo_sequence_remove_nullopt

#include "sequence_operator.hpp"
#include "filter.hpp"
#include "map.hpp"
#include "../option.hpp"
#include "../placeholder.hpp"

namespace ufo {
    constexpr auto remove_nullopt() noexcept {
        return filter([](const auto &o) {return static_cast<bool>(o);}) | map(*_);
    }
}

#endif
