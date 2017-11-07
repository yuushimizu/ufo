#ifndef ufo_sequence_take_while
#define ufo_sequence_take_while

#include "sequence.hpp"
#include "sequence_operator.hpp"
#include "../LambdaWrapper.hpp"

namespace ufo {
    template <typename F, typename Sequence>
    class TakenWhile : public sequence {
    private:
        LambdaWrapper<F> f_;
        Sequence sequence_;
        bool finished_ = false;
        
    public:
        constexpr explicit TakenWhile(F f, const Sequence &sequence) : f_(std::move(f)), sequence_(sequence) {
        }
        
        constexpr explicit TakenWhile(F f, Sequence &&sequence) noexcept : f_(std::move(f)), sequence_(std::move(sequence)) {
        }
        
        constexpr auto next() -> sequence_option_t<Sequence> {
            if (finished_) return nullopt;
            if (auto value = sequence_.next()) {
                if (f_(*value)) return value;
            }
            finished_ = true;
            return nullopt;
        }
    };
    
    template <typename F>
    constexpr auto take_while(F &&f) noexcept {
        return sequence_operator([](auto &&f, auto &&sequence) {
            return TakenWhile<std::decay_t<decltype(f)>, std::decay_t<decltype(sequence)>>(std::forward<decltype(f)>(f), std::forward<decltype(sequence)>(sequence));
        }, std::forward<F>(f));
    }
}

#endif
