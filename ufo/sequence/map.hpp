#ifndef ufo_sequence_map
#define ufo_sequence_map

#include <tuple>
#include "sequence.hpp"
#include "container_wrapper.hpp"
#include "sequence_operator.hpp"
#include "../lambda_wrapper.hpp"

namespace ufo {
    template <typename F, typename Sequence>
    class Mapped : public sequence {
    private:
        LambdaWrapper<F> f_;
        Sequence sequence_;
        
    public:
        constexpr Mapped(F f, const Sequence &sequence) : f_(std::move(f)), sequence_(sequence) {
        }
        
        constexpr Mapped(F f, Sequence &&sequence) noexcept : f_(std::move(f)), sequence_(std::move(sequence)) {
        }
        
        constexpr auto next() ->  decltype(sequence_.next().map(f_)) {
            return sequence_.next().map(f_);
        }
    };
    
    template <typename F>
    constexpr auto map(F f) noexcept {
        return sequence_operator([](auto &&f, auto &&sequence) {
            return Mapped<std::decay_t<decltype(f)>, std::decay_t<decltype(sequence)>>(std::forward<decltype(f)>(f), std::forward<decltype(sequence)>(sequence));
        }, std::move(f));
    }
}

#endif
