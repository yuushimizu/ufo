#ifndef ufo_range2_take
#define ufo_range2_take

#include "range.hpp"
#include "range_operator.hpp"

namespace ufo {
    template <typename Range>
    class Taken : public range {
    public:
        constexpr Taken(Range range, int rest) : range_(std::move(range)), rest_(rest) {
        }
        
        constexpr decltype(auto) first() const {
            return range_.first();
        }
        
        constexpr void pop() {
            range_.pop();
            --rest_;
        }
        
        constexpr bool empty() const {
            return rest_ <= 0 || range_.empty();
        }
        
    private:
        Range range_;
        int rest_;
    };
    
    constexpr auto take(int n) {
        return range_operator([n](auto range) constexpr {
            return Taken<decltype(range)>(std::move(range), n);
        });
    }
}

#endif
