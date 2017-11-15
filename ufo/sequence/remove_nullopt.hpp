#ifndef ufo_sequence_remove_nullopt
#define ufo_sequence_remove_nullopt

#include "sequence_operator.hpp"
#include "filter.hpp"
#include "map.hpp"
#include "../option.hpp"

namespace ufo {
    constexpr auto remove_nullopt() noexcept {
        return sequence_operator([](auto &&sequence) {
            return std::forward<decltype(sequence)>(sequence) | filter([](const auto &o) {return static_cast<bool>(o);}) | map([](auto &&o) -> decltype(auto) {return *std::forward<decltype(o)>(o);});
        });
    }
}

#endif
