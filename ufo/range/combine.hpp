#ifndef ufo_range_combine
#define ufo_range_combine

#include <utility>
#include <type_traits>
#include "../iterator.hpp"
#include "IteratorAdaptor.hpp"
#include "RangeFacade.hpp"

namespace ufo {
    namespace combine_detail {
        template <typename Base1, typename Base2, typename Combiner>
        struct Types : public IteratorAdaptorTypes<Base1> {
            using reference = std::result_of_t<Combiner(iterator_reference_t<Base1>, iterator_reference_t<Base2>)>;
            using value_type = std::decay_t<reference>;
            using pointer = value_type*;
        };
        
        template <typename Base1, typename Base2, typename Combiner>
        class CombineIterator : public IteratorFacade<CombineIterator<Base1, Base2, Combiner>, Types<Base1, Base2, Combiner>> {
        public:
            constexpr CombineIterator() = default;
            
            constexpr CombineIterator(Base1 base1, Base2 base2, Combiner combiner) : base1_(std::move(base1)), base2_(std::move(base2)), combiner_(std::move(combiner)) {
            }
            
        private:
            Base1 base1_;
            Base2 base2_;
            Combiner combiner_;
            
            friend IteratorAccess;
            
            constexpr decltype(auto) dereference() const {
                return combiner_(*base1_, *base2_);
            }
            
            constexpr void increment() {
                ++base1_;
                ++base2_;
            }
            
            constexpr void decrement() {
                --base1_;
                --base2_;
            }
            
            constexpr void advance(const iterator_difference_type_t<Base1> &difference) {
                base1_ += difference;
                base2_ += difference;
            }
            
            constexpr auto advanced(const iterator_difference_type_t<Base1> &difference) const {
                return CombineIterator(base1_ + difference, base2_ + difference, combiner_);
            }
            
            constexpr auto distance(const CombineIterator &rhs) const {
                return base1_ - rhs.base1_;
            }
            
            constexpr bool equal(const CombineIterator &rhs) const {
                return base1_ == rhs.base1_ || base2_ == rhs.base2_;
            }
            
            constexpr bool less_than(const CombineIterator &rhs) const {
                return base1_ < rhs.base1_;
            }
        };
        
        template <typename Base1, typename Base2, typename Combiner>
        class CombinedRange : public RangeFacade<CombinedRange<Base1, Base2, Combiner>> {
        public:
            constexpr CombinedRange(Base1 &&base1, Base2 &&base2, Combiner combiner)
            : base1_(std::forward<Base1>(base1))
            , base2_(std::forward<Base2>(base2))
            , combiner_(std::move(combiner))
            {
            }
            
        private:
            Base1 base1_;
            Base2 base2_;
            Combiner combiner_;
            
            friend RangeAccess;
            
            template <typename This, typename BaseIterator1, typename BaseIterator2>
            static constexpr auto make_iterator(This &t, BaseIterator1 &&base1, BaseIterator2 &&base2) {
                return CombineIterator<std::decay_t<BaseIterator1>, std::decay_t<BaseIterator2>, Combiner>(std::forward<BaseIterator1>(base1), std::forward<BaseIterator2>(base2), t.combiner_);
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
        
        template <typename Range1, typename Range2, typename Combiner>
        constexpr auto combine(Range1 &&range1, Range2 &&range2, Combiner combiner) {
            return CombinedRange<Range1, Range2, Combiner>(std::forward<Range1>(range1), std::forward<Range2>(range2), std::move(combiner));
        }
    }
    
    using combine_detail::combine;
}

#endif
