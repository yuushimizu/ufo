#ifndef ufo_sequence2_sequence_operator
#define ufo_sequence2_sequence_operator

#include "sequence.hpp"
#include "container_sequence.hpp"
#include "option_sequence.hpp"
#include "../type_traits.hpp"
#include "../functional.hpp"

namespace ufo::s2 {
    template <typename S, enable_if_t<is_sequence_v<std::decay_t<S>>> = enabler>
    constexpr decltype(auto) ensure_sequence(S &&seq) {
        return std::forward<S>(seq);
    }
    
    template <typename T, enable_if_t<!is_sequence_v<std::decay_t<T>>> = enabler>
    constexpr auto ensure_sequence(T &&container) {
        return container_sequence(std::forward<T>(container));
    }
    
    template <typename T>
    constexpr auto ensure_sequence(option<T> option) {
        return option_sequence(std::move(option));
    }
    
    template <typename F>
    class sequence_operator {
    public:
        constexpr sequence_operator(F f) noexcept : f_(std::move(f)) {
        }
        
        template <typename ... Seqs>
        constexpr decltype(auto) operator()(Seqs && ... seqs) const & {
            return f_(ensure_sequence(std::forward<Seqs>(seqs)) ...);
        }
        
        template <typename ... Seqs>
        constexpr decltype(auto) operator()(Seqs && ... seqs) & {
            return f_(ensure_sequence(std::forward<Seqs>(seqs)) ...);
        }
        
        template <typename ... Seqs>
        constexpr decltype(auto) operator()(Seqs && ... seqs) && {
            return std::move(f_)(ensure_sequence(std::forward<Seqs>(seqs)) ...);
        }
        
    private:
        ufo::function_wrapper<F> f_;
    };
    
    template <typename T>
    constexpr inline const bool is_sequence_operator_v = is_instantiation_of_v<sequence_operator, T>;
    
    template <typename S, typename Op, enable_if_t<is_sequence_operator_v<std::decay_t<Op>> && !is_sequence_operator_v<std::decay_t<S>>> = enabler>
    constexpr decltype(auto) operator|(S &&seq, Op &&op) {
        return std::forward<Op>(op)(std::forward<S>(seq));
    }
    
    template <typename LHS, typename RHS, enable_if_t<is_sequence_operator_v<std::decay_t<LHS>> && is_sequence_operator_v<std::decay_t<RHS>>> = enabler>
    constexpr decltype(auto) operator|(LHS lhs, RHS rhs) {
        return sequence_operator(partial([](auto &&lhs, auto &&rhs, auto && ... seqs) {
            return std::forward<decltype(lhs)>(lhs)(std::forward<decltype(seqs)>(seqs) ...) | std::forward<decltype(rhs)>(rhs);
        }, std::move(lhs), std::move(rhs)));
    }
}

#endif
