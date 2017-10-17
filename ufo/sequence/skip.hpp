#ifndef ufo_sequence_skip
#define ufo_sequence_skip

#include "sequence.hpp"
#include "sequence_operator.hpp"

namespace ufo {
    template <typename Sequence>
    class Skipped : public sequence {
    private:
        int rest_;
        Sequence sequence_;
        
    public:
        constexpr Skipped(int n, const Sequence &sequence) : rest_(n), sequence_(sequence) {
        }
        
        constexpr Skipped(int n, Sequence &&sequence) noexcept : rest_(n), sequence_(std::move(sequence)) {
        }
        
        constexpr auto next() -> sequence_option_t<Sequence> {
            while (rest_ > 0) {
                sequence_.next();
                --rest_;
            }
            return sequence_.next();
        }
    };
    
    constexpr auto skip(int n) noexcept {
        return sequence_operator([n](auto &&sequence) constexpr noexcept  {
            return Skipped<std::decay_t<decltype(sequence)>>(n, std::forward<decltype(sequence)>(sequence));
        });
    }
}

#endif
