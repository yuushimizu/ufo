#ifndef ufo_range2_filter
#define ufo_range2_filter

#include "range.hpp"
#include "range_operator.hpp"
#include "core.hpp"
#include "../optional.hpp"
#include "../TD.hpp"

namespace ufo {
    template <typename F, typename Range>
    class Filtered : public range {
    private:
        using reference = decltype(std::declval<Range>() | first);
        
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
        
        void normalize() {
            while (!(range_ | is_empty)) {
                current_ = range_.first();
                if (f_(*current_)) return;
                range_.pop();
            }
        }
    };
    
    template <typename F>
    constexpr auto filter(F f) {
        return range_operator([f](auto range) constexpr {
            return Filtered<F, decltype(range)>(f, std::move(range));
        });
    }
}

#endif

