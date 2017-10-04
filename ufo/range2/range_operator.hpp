#ifndef ufo_range2_range_operator
#define ufo_range2_range_operator

#include "range.hpp"
#include "container_wrapper.hpp"
#include "../type_traits.hpp"

namespace ufo {
    template <typename F>
    class RangeOperator final {
    public:
        constexpr RangeOperator(F f) : f_(std::move(f)) {
        }
        
        template <typename Range, enable_if_t<std::is_base_of_v<range, std::decay_t<Range>>> = nullptr>
        constexpr decltype(auto) operator()(Range &&range) const & {
            return f_(std::forward<Range>(range));
        }
        
        template <typename Range, enable_if_t<std::is_base_of_v<range, std::decay_t<Range>>> = nullptr>
        constexpr decltype(auto) operator()(Range &&range) & {
            return f_(std::forward<Range>(range));
        }
        
        template <typename Range, enable_if_t<std::is_base_of_v<range, std::decay_t<Range>>> = nullptr>
        constexpr decltype(auto) operator()(Range &&range) && {
            return std::move(f_)(std::forward<Range>(range));
        }
        
        template <typename Range, enable_if_t<!std::is_base_of_v<range, std::decay_t<Range>>> = nullptr>
        constexpr decltype(auto) operator()(Range &&range) const & {
            return (*this)(container_wrapper(std::forward<Range>(range)));
        }
        
        template <typename Range, enable_if_t<!std::is_base_of_v<range, std::decay_t<Range>>> = nullptr>
        constexpr decltype(auto) operator()(Range &&range) & {
            return (*this)(container_wrapper(std::forward<Range>(range)));
        }
        
        template <typename Range, enable_if_t<!std::is_base_of_v<range, std::decay_t<Range>>> = nullptr>
        constexpr decltype(auto) operator()(Range &&range) && {
            return std::move(*this)(container_wrapper(std::forward<Range>(range)));
        }

    private:
        F f_;
    };
    
    template <typename Range, typename Op, enable_if_t<is_instantiation_of_v<RangeOperator, std::decay_t<Op>>> = nullptr>
    constexpr decltype(auto) operator|(Range &&range, Op &&op) {
        return std::forward<Op>(op)(std::forward<Range>(range));
    }
    
    template <typename F>
    constexpr auto range_operator(F f) {
        return RangeOperator<F>(std::move(f));
    }
}

#endif
