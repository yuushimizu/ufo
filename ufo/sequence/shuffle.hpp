#ifndef ufo_sequence_shuffle
#define ufo_sequence_shuffle

#include <algorithm>
#include "sequence_operator.hpp"
#include "../container.hpp"
#include "to_vector.hpp"

namespace ufo {
    template <typename Random>
    constexpr auto shuffle(Random &random) {
        return sequence_operator([&random](auto &&sequence) {
            auto result = std::forward<decltype(sequence)>(sequence) | to_vector();
            std::shuffle(adl_begin(result), adl_end(result), random);
            return result;
        });
    }
}

#endif
