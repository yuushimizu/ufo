#ifndef ufo_sequence_sequence_wrapper
#define ufo_sequence_sequence_wrapper

#include <tuple>
#include "sequence.hpp"
#include "sequence_operator.hpp"
#include "../LambdaWrapper.hpp"

namespace ufo {
    template <typename F, typename ... Sequences>
    class WrappedSequence : public sequence {
    private:
        LambdaWrapper<F> f_;
        std::tuple<Sequences ...> sequences_;
        
    public:
        template <typename ... Seqs>
        constexpr explicit WrappedSequence(F f, Seqs && ... sequences) : f_(std::move(f)), sequences_ {std::forward<Seqs>(sequences) ...} {
        }
        
        constexpr auto next() {
            return std::apply(f_, sequences_);
        }
    };
    
    template <typename F, typename ... Sequences>
    constexpr auto wrapped_sequence(F &&f, Sequences ... sequences) {
        return WrappedSequence<std::decay_t<F>, std::decay_t<Sequences> ...>(std::forward<F>(f), std::forward<Sequences>(sequences) ...);
    }
    
    template <typename F>
    constexpr auto sequence_wrapper(F &&f) noexcept {
        return sequence_operator([](auto &&f, auto && ... sequences) {
            return wrapped_sequence(std::forward<decltype(f)>(f), std::forward<decltype(sequences)>(sequences) ...);
        }, std::forward<F>(f));
    }
}

#endif
