#ifndef ufo_sequence_take
#define ufo_sequence_take

#include "sequence.hpp"
#include "sequence_operator.hpp"
#include "../option.hpp"

namespace ufo {
    template <typename Sequence>
    class Taken : public sequence {
    private:
        int rest_;
        Sequence sequence_;
        
    public:
        constexpr Taken(int rest, Sequence sequence) : rest_(rest), sequence_(std::move(sequence)) {
        }
        
        constexpr auto next() -> sequence_option_t<Sequence> {
            if (rest_ <= 0) return nullopt;
            --rest_;
            return sequence_.next();
        }
    };
    
    constexpr auto take(int n) {
        return sequence_operator([n](auto sequence) constexpr {
            return Taken<decltype(sequence)>(n, std::move(sequence));
        });
    }
}

#endif
