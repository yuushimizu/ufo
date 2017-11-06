#ifndef ufo_sequence_sequence_operator
#define ufo_sequence_sequence_operator

#include "sequence.hpp"
#include "container_wrapper.hpp"
#include "../type_traits.hpp"

namespace ufo {
    template <typename F, typename ... FixedArgs>
    class SequenceOperator final {
    private:
        F f_;
        std::tuple<FixedArgs ...> fixed_args_;
        
    public:
        constexpr explicit SequenceOperator(F f, std::tuple<FixedArgs ...> fixed_args) noexcept : f_(std::move(f)), fixed_args_(std::move(fixed_args)) {
        }
        
        template <typename ... Sequences>
        constexpr decltype(auto) operator()(Sequences && ... sequences) const & {
            return std::apply(f_, std::tuple_cat(fixed_args_, std::make_tuple(container_wrapper(std::forward<Sequences>(sequences)) ...)));
        }
        
        template <typename ... Sequences>
        constexpr decltype(auto) operator()(Sequences && ... sequences) & {
            return std::apply(f_, std::tuple_cat(fixed_args_, std::make_tuple(container_wrapper(std::forward<Sequences>(sequences)) ...)));
        }
        
        template <typename ... Sequences>
        constexpr decltype(auto) operator()(Sequences && ... sequences) && {
            return std::apply(std::move(f_), std::tuple_cat(std::move(fixed_args_), std::make_tuple(container_wrapper(std::forward<Sequences>(sequences)) ...)));
        }
    };
    
    template <typename Sequence, typename Op, enable_if_t<!is_instantiation_of_v<SequenceOperator, Sequence> && is_instantiation_of_v<SequenceOperator, std::decay_t<Op>>> = nullptr>
    constexpr decltype(auto) operator|(Sequence &&sequence, Op &&op) {
        return std::forward<Op>(op)(std::forward<Sequence>(sequence));
    }
    
    template <typename F, typename ... FixedArgs>
    constexpr auto sequence_operator(F f, FixedArgs ... fixed_args) noexcept {
        return SequenceOperator<F, FixedArgs ...>(std::move(f), {std::move(fixed_args) ...});
    }
    
    template <typename LHSOp, typename RHSOp, enable_if_t<is_instantiation_of_v<SequenceOperator, std::decay_t<LHSOp>> && is_instantiation_of_v<SequenceOperator, std::decay_t<RHSOp>>> = nullptr>
    constexpr decltype(auto) operator|(LHSOp &&lhs, RHSOp &&rhs) {
        return sequence_operator([](auto &&lhs, auto &&rhs, auto && ... sequences) {
            return std::forward<LHSOp>(lhs)(std::forward<decltype(sequences)>(sequences) ...) | std::forward<RHSOp>(rhs);
        }, std::forward<LHSOp>(lhs), std::forward<RHSOp>(rhs));
    }
}

#endif
