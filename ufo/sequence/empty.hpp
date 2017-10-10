#ifndef ufo_sequence_empty
#define ufo_sequence_empty

#include "sequence.hpp"
#include "../option.hpp"

namespace ufo {
    template <typename T>
    class Empty : public sequence {
    public:
        constexpr option<T> next() const noexcept {
            return nullopt;
        }
    };
    
    template <typename T>
    constexpr const auto empty = Empty<T> {};
}

#endif
