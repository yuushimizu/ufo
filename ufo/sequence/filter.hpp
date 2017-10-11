#ifndef ufo_sequence_filter
#define ufo_sequence_filter

#include "sequence.hpp"
#include "sequence_operator.hpp"
#include "../lambda_wrapper.hpp"

namespace ufo {
    template <typename F, typename Sequence>
    class Filtered : public sequence {
    private:
        LambdaWrapper<F> f_;
        Sequence sequence_;
        
    public:
        constexpr Filtered(F f, const Sequence &sequence) : f_(std::move(f)), sequence_(sequence) {
        }
        
        constexpr Filtered(F f, Sequence &&sequence) noexcept : f_(std::move(f)), sequence_(std::move(sequence)) {
        }
        
        constexpr auto next() -> sequence_option_t<Sequence> {
            while (true) {
                if (auto value = sequence_.next()) {
                    if (f_(*value)) return value;
                } else {
                    return nullopt;
                }
            }
        }
    };
    
    template <typename F>
    constexpr auto filter(F &&f) noexcept {
        return sequence_operator([](auto &&f, auto &&sequence) {
            return Filtered<std::decay_t<decltype(f)>, std::decay_t<decltype(sequence)>>(std::forward<decltype(f)>(f), std::forward<decltype(sequence)>(sequence));
        }, std::forward<F>(f));
    }
}

#endif

