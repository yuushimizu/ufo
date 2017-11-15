#ifndef ufo_sequence_as_vector
#define ufo_sequence_as_vector

#include <vector>
#include "sequence_operator.hpp"
#include "as_container.hpp"

namespace ufo {
    constexpr auto as_vector() noexcept {
        return sequence_operator([](auto &&sequence) -> std::vector<std::decay_t<sequence_element_t<decltype(sequence)>>> {
            return std::forward<decltype(sequence)>(sequence) | as_container();
        });
    }
}

#endif
