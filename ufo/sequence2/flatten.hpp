#ifndef ufo_sequence2_flatten
#define ufo_sequence2_flatten

#include "sequence_operator.hpp"
#include "sequence.hpp"
#include "../option.hpp"

namespace ufo::s2 {
    constexpr auto flatten() noexcept {
        return sequence_operator([](auto &&seq) {
            return sequence([seq = std::forward<decltype(seq)>(seq)]() {
                auto next_sequence = [](auto &&i) {
                    return std::forward<decltype(i)>(i).next().map([](auto &&seq) {return ensure_sequence(std::forward<decltype(seq)>(seq));});
                };
                using seq_option_type = decltype(next_sequence(seq.begin()));
                using inner_iterator_type = decltype(next_sequence(seq.begin())->begin());
                return [next_sequence, base = seq.begin(), inner_seq = seq_option_type {}, inner_iterator = option<inner_iterator_type> {}]() mutable {
                    while (true) {
                        if (inner_iterator) {
                            auto next = inner_iterator->next();
                            if (next) return next;
                        }
                        inner_seq = next_sequence(base);
                        if (!inner_seq) return decltype(inner_iterator->next()) {};
                        inner_iterator.emplace(inner_seq->emitter());
                    }
                };
            });
        });
    }
}

#endif
