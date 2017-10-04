#ifndef ufo_sequence_as_vector
#define ufo_sequence_as_vector

#include <vector>
#include "sequence_operator.hpp"
#include "as_container.hpp"

namespace ufo {
    constexpr const auto as_vector = sequence_operator([](auto &&sequence) -> std::vector<std::decay_t<decltype(std::forward<decltype(sequence)>(sequence).first())>> {
        return std::forward<decltype(sequence)>(sequence) | as_container;
    });
}

#endif
