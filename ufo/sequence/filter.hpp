#ifndef ufo_sequence_filter
#define ufo_sequence_filter

#include "sequence_wrapper.hpp"

namespace ufo {
    template <typename F>
    constexpr auto filter(F f) noexcept {
        return sequence_wrapper([f = std::move(f)](auto &&sequence) constexpr -> sequence_option_t<decltype(sequence)> {
            while (true) {
                if (auto value = sequence.next()) {
                    if (f(*value)) return value;
                } else {
                    return nullopt;
                }
            }
        });
    }
}

#endif

