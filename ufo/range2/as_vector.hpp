#ifndef ufo_range2_as_vector
#define ufo_range2_as_vector

#include <vector>
#include "range_operator.hpp"
#include "as_container.hpp"

namespace ufo {
    constexpr const auto as_vector = range_operator([](auto &&range) -> std::vector<std::decay_t<decltype(std::forward<decltype(range)>(range).first())>> {
        return std::forward<decltype(range)>(range) | as_container;
    });
}

#endif
