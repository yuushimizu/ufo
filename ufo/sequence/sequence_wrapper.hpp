#ifndef ufo_sequence_sequence_wrapper
#define ufo_sequence_sequence_wrapper

#include <tuple>
#include "sequence.hpp"
#include "../lambda_wrapper.hpp"

namespace ufo {
    template <typename F, typename ... Sequences>
    class SequenceWrapper : public sequence {
    private:
        lambda_wrapper<F> f_;
        std::tuple<Sequences ...> sequences_;
        
    public:
        template <typename ... Seqs>
        constexpr explicit SequenceWrapper(F f, Seqs && ... sequences) : f_(std::move(f)), sequences_ {std::forward<Seqs>(sequences) ...} {
        }
        
        constexpr auto next() {
            return std::apply(f_, sequences_);
        }
    };
    
    template <typename F, typename ... Sequences>
    constexpr auto sequence_wrapper(F &&f, Sequences ... sequences) {
        return SequenceWrapper<std::decay_t<F>, std::decay_t<Sequences> ...>(std::forward<F>(f), std::forward<Sequences>(sequences) ...);
    }
}

#endif
