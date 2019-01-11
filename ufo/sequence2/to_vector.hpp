#ifndef ufo_sequence2_to_vector
#define ufo_sequence2_to_vector

#include "sequence_operator.hpp"
#include "to_container.hpp"
#include <vector>

namespace ufo::s2 {
    constexpr auto to_vector() noexcept {
        return sequence_operator([](auto &&seq) -> std::vector<std::remove_cv_t<std::remove_reference_t<decltype(*seq.begin().next())>>> {
            return std::forward<decltype(seq)>(seq) | to_container();
        });
    }
}

#endif
