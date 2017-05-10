#ifndef ufo_range_join
#define ufo_range_join

#include <utility>
#include <type_traits>
#include "iterator.hpp"
#include "IteratorAdaptor.hpp"
#include "RangeFacade.hpp"

namespace ufo {
    namespace join_detail {
        template <typename Base1, typename Base2>
        class JoinIterator : public IteratorFacade<JoinIterator<Base1, Base2>, IteratorAdaptorTypes<Base1>> {
        public:
            constexpr JoinIterator() = default;
            
            constexpr JoinIterator(Base1 base1, Base2 base2, Base1 base1_end) : base1_(std::move(base1)), base2_(std::move(base2)), base1_end_(std::move(base1_end)) {
            }
            
        private:
            Base1 base1_;
            Base2 base2_;
            Base1 base1_end_;
            
            friend IteratorAccess;
            
            constexpr decltype(auto) dereference() const {
                return base1_ == base1_end_ ? *base2_ : *base1_;
            }
            
            constexpr void increment() {
                if (base1_ != base1_end_) {
                    ++base1_;
                } else {
                    ++base2_;
                }
            }
            
            constexpr auto distance(const JoinIterator &rhs) const {
                return (base1_ - rhs.base1_) + (base2_ - rhs.base2_);
            }
            
            constexpr bool equal(const JoinIterator &rhs) const {
                return base1_ == rhs.base1_ && base2_ == rhs.base2_;
            }
            
            constexpr bool less_than(const JoinIterator &rhs) const {
                return base1_ < rhs.base1_ || base2_ < rhs.base2_;
            }
        };
        
        template <typename Base1, typename Base2>
        class JoinedRange : public RangeFacade<JoinedRange<Base1, Base2>> {
        public:
            constexpr JoinedRange(Base1 &&base1, Base2 &&base2) : base1_(std::forward<Base1>(base1)), base2_(std::forward<Base2>(base2)) {
            }
            
        private:
            Base1 base1_;
            Base2 base2_;
            
            friend RangeAccess;
            
            template <typename This, typename BaseIterator1, typename BaseIterator2>
            static constexpr auto make_iterator(This &t, BaseIterator1 &&base1, BaseIterator2 &&base2) {
                return JoinIterator<std::decay_t<BaseIterator1>, std::decay_t<BaseIterator2>>(std::forward<BaseIterator1>(base1), std::forward<BaseIterator2>(base2), adl_end(t.base1_));
            }
            
            template <typename This>
            static constexpr auto make_begin(This &t) {
                return make_iterator(t, adl_begin(t.base1_), adl_begin(t.base2_));
            }
            
            template <typename This>
            static constexpr auto make_end(This &t) {
                return make_iterator(t, adl_end(t.base1_), adl_end(t.base2_));
            }
        };
        
        template <typename Range>
        constexpr auto join(Range &&range) {
            return std::forward<Range>(range);
        }
        
        template <typename Range1, typename Range2>
        constexpr auto join(Range1 &&range1, Range2 &&range2) {
            return JoinedRange<Range1, Range2>(std::forward<Range1>(range1), std::forward<Range2>(range2));
        }
        
        template <typename Range1, typename ... Rest>
        constexpr auto join(Range1 &&range1, Rest && ... rest) {
            return join(std::forward<Range1>(range1), join(std::forward<Rest>(rest) ...));
        }
    }
    
    using join_detail::join;
}

#endif
