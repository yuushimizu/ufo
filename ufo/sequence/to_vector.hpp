#ifndef ufo_sequence_to_vector
#define ufo_sequence_to_vector

#include <vector>
#include "sequence_operator.hpp"
#include "to_container.hpp"

namespace ufo {
    constexpr auto to_vector() noexcept {
        return sequence_operator([](auto &&sequence) -> std::vector<std::decay_t<sequence_element_t<decltype(sequence)>>> {
            return std::forward<decltype(sequence)>(sequence) | to_container();
        });
    }
}

#endif
