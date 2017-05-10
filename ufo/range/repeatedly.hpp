#ifndef ufo_range_repeatedly
#define ufo_range_repeatedly

#include <utility>
#include "iterate.hpp"

namespace ufo {
    template <typename Generator>
    constexpr auto repeatedly(Generator generator) {
        auto initial_value = generator();
        return iterate(std::move(initial_value), [generator = std::move(generator)](auto &&) mutable -> decltype(auto) {return generator();});
    }
}

#endif
