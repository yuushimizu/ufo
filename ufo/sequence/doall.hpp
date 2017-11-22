#ifndef ufo_sequence_doall
#define ufo_sequence_doall

#include "sequence_operator.hpp"

namespace ufo {
    constexpr auto doall() {
        return sequence_operator([](auto &&sequence) constexpr {
            while (sequence.next());
        });
    }
}

#endif
