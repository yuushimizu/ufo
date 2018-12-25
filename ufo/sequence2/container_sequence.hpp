#ifndef ufo_sequence2_container_sequence
#define ufo_sequence2_container_sequence

#include "sequence.hpp"
#include "../box.hpp"
#include "../iterator.hpp"
#include "../option.hpp"
#include "../functional.hpp"

namespace ufo::s2 {
    template <typename T>
    constexpr auto container_sequence(T &&container) {
        return sequence([container = forward_box<T>(container)]() {
            return [i = adl_begin(container.get()), end = adl_end(container.get())]() mutable {
                if (i != end) {
                    auto element = forward_option(*i);
                    ++i;
                    return element;
                } else {
                    return decltype(forward_option(*i)) {};
                }
            };
        });
    }
}

#endif
