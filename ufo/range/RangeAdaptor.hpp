#ifndef ufo_range_RangeAdaptor
#define ufo_range_RangeAdaptor

#include <utility>
#include <type_traits>
#include "iterator.hpp"
#include "RangeFacade.hpp"

namespace ufo {
    template <typename Derived>
    class RangeOperation {
    protected:
        constexpr RangeOperation() = default;
        
        ~RangeOperation() = default;
        
        constexpr RangeOperation(const RangeOperation &) = default;
        
        constexpr RangeOperation(RangeOperation &&) = default;
        
        constexpr RangeOperation &operator=(const RangeOperation &) = default;
        
        constexpr RangeOperation &operator=(RangeOperation &&) = default;
        
    private:
        template <typename Range>
        friend constexpr auto operator|(Range &&range, const Derived &derived) {
            return derived(std::forward<Range>(range));
        }
        
        template <typename Range>
        friend constexpr auto operator|(Range &&range, Derived &derived) {
            return derived(std::forward<Range>(range));
        }
        
        template <typename Range>
        friend constexpr auto operator|(Range &&range, Derived &&derived) {
            return derived(std::forward<Range>(range));
        }
    };
    
    class RangeAdaptorAccess {
    public:
        template <typename Range, typename BaseIterator>
        static constexpr decltype(auto) make_iterator(Range &&range, BaseIterator &&base) {
            return std::decay_t<Range>::make_iterator(std::forward<Range>(range), std::forward<BaseIterator>(base));
        }
    };
    
    template <typename Derived, typename Base>
    class RangeAdaptor : public RangeFacade<Derived> {
    protected:
        constexpr RangeAdaptor() = default;
        
        constexpr RangeAdaptor(Base &&base) : base_(std::forward<Base>(base)) {
        }
        
        ~RangeAdaptor() = default;
        
        constexpr RangeAdaptor(const RangeAdaptor &) = default;
        
        constexpr RangeAdaptor(RangeAdaptor &&) = default;
        
        constexpr RangeAdaptor &operator=(const RangeAdaptor &) = default;
        
        constexpr RangeAdaptor &operator=(RangeAdaptor &) = default;
        
        constexpr const Base &base() const {
            return base_;
        }
        
        constexpr Base &base() {
            return base_;
        }
        
    private:
        Base base_;
        
        friend RangeAccess;
        
        template <typename Range>
        static constexpr auto make_begin(Range &range) {
            return RangeAdaptorAccess::make_iterator(range, adl_begin(range.base_));
        }
        
        template <typename Range>
        static constexpr auto make_end(Range &range) {
            return RangeAdaptorAccess::make_iterator(range, adl_end(range.base_));
        }
    };
}

#endif
