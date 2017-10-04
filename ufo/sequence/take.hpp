#ifndef ufo_sequence_take
#define ufo_sequence_take

#include "sequence.hpp"
#include "sequence_operator.hpp"

namespace ufo {
    template <typename Sequence>
    class Taken : public sequence {
    public:
        constexpr Taken(int rest, Sequence sequence) : rest_(rest), sequence_(std::move(sequence)) {
        }
        
        constexpr decltype(auto) first() const {
            return sequence_.first();
        }
        
        constexpr void pop() {
            sequence_.pop();
            --rest_;
        }
        
        constexpr bool empty() const {
            return rest_ <= 0 || sequence_.empty();
        }
        
    private:
        int rest_;
        Sequence sequence_;
    };
    
    constexpr auto take(int n) {
        return sequence_operator([n](auto sequence) constexpr {
            return Taken<decltype(sequence)>(n, std::move(sequence));
        });
    }
}

#endif
