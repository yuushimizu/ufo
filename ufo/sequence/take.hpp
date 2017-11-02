#ifndef ufo_sequence_take
#define ufo_sequence_take

#include "sequence.hpp"
#include "sequence_operator.hpp"

namespace ufo {
    template <typename Sequence>
    class Taken : public sequence {
    private:
        int rest_;
        Sequence sequence_;
        
    public:
        constexpr explicit Taken(int rest, const Sequence &sequence) : rest_(rest), sequence_(sequence) {
        }
        
        constexpr explicit Taken(int rest, Sequence &&sequence) noexcept : rest_(rest), sequence_(std::move(sequence)) {
        }
        
        constexpr auto next() -> sequence_option_t<Sequence> {
            if (rest_ <= 0) return nullopt;
            if (auto value = sequence_.next()) {
                --rest_;
                return value;
            } else {
                return nullopt;
            }
        }
    };
    
    constexpr auto take(int n) noexcept {
        return sequence_operator([n](auto &&sequence) constexpr noexcept {
            return Taken<std::decay_t<decltype(sequence)>>(n, std::forward<decltype(sequence)>(sequence));
        });
    }
}

#endif
