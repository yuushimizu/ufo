#ifndef ufo_range_RangeFacade
#define ufo_range_RangeFacade

#include <utility>
#include <type_traits>

namespace ufo {
    class RangeAccess {
    public:
        template <typename Range>
        static constexpr decltype(auto) begin(Range &&range) {
            return std::decay_t<Range>::make_begin(std::forward<Range>(range));
        }
        
        template <typename Range>
        static constexpr decltype(auto) end(Range &&range) {
            return std::decay_t<Range>::make_end(std::forward<Range>(range));
        }
    };
    
    template <typename Derived>
    class RangeFacade {
    protected:
        constexpr RangeFacade() = default;
        
        ~RangeFacade() = default;
        
        constexpr RangeFacade(const RangeFacade &) = default;
        
        constexpr RangeFacade(RangeFacade &&) = default;
        
        RangeFacade &operator=(const RangeFacade &) = default;
        
        RangeFacade &operator=(RangeFacade &&) = default;
        
        constexpr Derived &derived() noexcept {
            return *static_cast<Derived*>(this);
        }
        
        constexpr const Derived &derived() const noexcept {
            return *static_cast<const Derived *>(this);
        }
        
    public:
        constexpr auto begin() const {
            return RangeAccess::begin(derived());
        }
        
        constexpr auto begin() {
            return RangeAccess::begin(derived());
        }
        
        constexpr auto end() const {
            return RangeAccess::end(derived());
        }
        
        constexpr auto end() {
            return RangeAccess::end(derived());
        }
    };
}

#endif
