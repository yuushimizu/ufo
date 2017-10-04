#ifndef ufo_sequence_core
#define ufo_sequence_core

#include "sequence_operator.hpp"

namespace ufo {
    constexpr const auto first = sequence_operator([](auto &&sequence) constexpr -> decltype(auto) {
        return std::forward<decltype(sequence)>(sequence).first();
    });
    
    constexpr const auto rest = sequence_operator([](auto &&sequence) constexpr -> decltype(auto) {
        auto result = std::forward<decltype(sequence)>(sequence);
        result.pop();
        return result;
    });
    
    constexpr const auto is_empty = sequence_operator([](const auto &sequence) constexpr -> bool {
        return sequence.empty();
    });
}

#endif
