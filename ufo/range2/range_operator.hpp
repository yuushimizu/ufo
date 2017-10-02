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
            return f_(container_wrapper(std::forward<Range>(range)));
        }
        
        template <typename Range, enable_if_t<!std::is_base_of_v<range, std::decay_t<Range>>> = nullptr>
        constexpr decltype(auto) operator()(Range &&range) & {
            return f_(container_wrapper(std::forward<Range>(range)));
        }
        
        template <typename Range, enable_if_t<!std::is_base_of_v<range, std::decay_t<Range>>> = nullptr>
        constexpr decltype(auto) operator()(Range &&range) && {
            return std::move(f_)(container_wrapper(std::forward<Range>(range)));
        }

    private:
        const F f_;
    };
    
    template <typename Range, typename F>
    constexpr decltype(auto) operator|(Range &&range, const RangeOperator<F> &op) {
        return op(std::forward<Range>(range));
    }
    
    template <typename Range, typename F>
    constexpr decltype(auto) operator|(Range &&range, RangeOperator<F> &op) {
        return op(std::forward<Range>(range));
    }
    
    template <typename Range, typename F>
    constexpr decltype(auto) operator|(Range &&range, RangeOperator<F> &&op) {
        return std::move(op)(std::forward<Range>(range));
    }
    
    template <typename F>
    constexpr auto range_operator(F f) {
        return RangeOperator<F>(std::move(f));
    }
}

#endif
