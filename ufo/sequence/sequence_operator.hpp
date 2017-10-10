#ifndef ufo_sequence_sequence_operator
#define ufo_sequence_sequence_operator

#include "sequence.hpp"
#include "container_wrapper.hpp"
#include "../type_traits.hpp"

namespace ufo {
    template <typename F>
    class SequenceOperator final {
    private:
        F f_;
        
    public:
        constexpr SequenceOperator(F f) noexcept : f_(std::move(f)) {
        }
        
        template <typename ... Sequences>
        constexpr decltype(auto) operator()(Sequences && ... sequences) const & {
            return f_(container_wrapper(std::forward<Sequences>(sequences)) ...);
        }
        
        template <typename ... Sequences>
        constexpr decltype(auto) operator()(Sequences && ... sequences) & {
            return f_(container_wrapper(std::forward<Sequences>(sequences)) ...);
        }
        
        template <typename ... Sequences>
        constexpr decltype(auto) operator()(Sequences && ... sequences) && {
            return std::move(f_)(container_wrapper(std::forward<Sequences>(sequences)) ...);
        }
    };
    
    template <typename Sequence, typename Op, enable_if_t<is_instantiation_of_v<SequenceOperator, std::decay_t<Op>>> = nullptr>
    constexpr decltype(auto) operator|(Sequence &&sequence, Op &&op) {
        return std::forward<Op>(op)(std::forward<Sequence>(sequence));
    }
    
    template <typename F>
    constexpr auto sequence_operator(F f) noexcept {
        return SequenceOperator<F>(std::move(f));
    }
}

#endif
