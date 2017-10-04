#ifndef ufo_range2_filter
#define ufo_range2_filter

#include "range.hpp"
#include "range_operator.hpp"
#include "../optional.hpp"

namespace ufo {
    template <typename F, typename Range>
    class Filtered : public range {
    private:
        using reference = decltype(std::declval<Range>().first());
        
    public:
        constexpr Filtered(F f, Range range) : f_(std::move(f)), range_(std::move(range)) {
            normalize();
        }
        
        constexpr reference first() const {
            return *current_;
        }
        
        constexpr void pop() {
            range_.pop();
            normalize();
        }
        
        constexpr bool empty() const {
            return range_.empty();
        }
        
    private:
        F f_;
        Range range_;
        optional<reference> current_;
        
        constexpr void normalize() {
            while (!(range_.empty())) {
                current_ = range_.first();
                if (f_(*current_)) return;
                range_.pop();
            }
        }
    };
    
    template <typename F>
    class Filter {
    public:
        constexpr Filter(F f) : f_(std::move(f)) {
        }
        
        template <typename Range>
        constexpr auto operator()(Range &&range) const & {
            return Filtered<F, Range>(f_, std::forward<Range>(range));
        }
        
        template <typename Range>
        constexpr auto operator()(Range &&range) & {
            return Filtered<F, Range>(f_, std::forward<Range>(range));
        }
        
        template <typename Range>
        constexpr auto operator()(Range &&range) && {
            return Filtered<F, Range>(std::move(f_), std::forward<Range>(range));
        }
        
    private:
        F f_;
    };
    
    template <typename F>
    constexpr auto filter(F f) {
        return range_operator(Filter<F>(std::move(f)));
    }
}

#endif

