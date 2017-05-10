#ifndef ufo_range_filtered
#define ufo_range_filtered

#include <utility>
#include <iterator>
#include "../iterator.hpp"
#include "IteratorAdaptor.hpp"
#include "RangeAdaptor.hpp"

namespace ufo {
    namespace filtered_detail {
        template <typename Base>
        struct Types : public IteratorAdaptorTypes<Base> {
            using iterator_category = std::forward_iterator_tag;
        };
        
        template <typename Base, typename Predicate>
        class FilteredIterator;
        
        template <typename Base, typename Predicate>
        using IteratorBase = IteratorAdaptor<FilteredIterator<Base, Predicate>, Base, Types<Base>>;
        
        template <typename Base, typename Predicate>
        class FilteredIterator : public IteratorBase<Base, Predicate> {
        private:
            constexpr void normalize() {
                while (this->base() != base_end_ && !predicate_(this->dereference())) {
                    IteratorBase<Base, Predicate>::increment();
                }
            }
            
        public:
            constexpr FilteredIterator() = default;
            
            constexpr FilteredIterator(Base base, Base base_end, Predicate predicate)
            : IteratorBase<Base, Predicate>(std::move(base))
            , base_end_(std::move(base_end))
            , predicate_(std::move(predicate))
            {
                normalize();
            }
            
        private:
            Base base_end_;
            Predicate predicate_;
            
            friend IteratorAccess;
            
            constexpr void increment() {
                IteratorBase<Base, Predicate>::increment();
                normalize();
            }
        };
        
        template <typename Base, typename Predicate>
        class FilteredRange : public RangeAdaptor<FilteredRange<Base, Predicate>, Base> {
        public:
            constexpr FilteredRange(Base &&base, Predicate predicate)
            : RangeAdaptor<FilteredRange<Base, Predicate>, Base>(std::forward<Base>(base))
            , predicate_(std::move(predicate)) {
            }
            
        private:
            Predicate predicate_;
            
            friend RangeAdaptorAccess;
            
            template <typename This, typename BaseIterator>
            static constexpr auto make_iterator(This &t, BaseIterator &&base) {
                return FilteredIterator<std::decay_t<BaseIterator>, Predicate>(std::forward<BaseIterator>(base), adl_end(t.base()), t.predicate_);
            }
        };
        
        template <typename Range, typename Predicate>
        constexpr auto filter(Range &&range, Predicate predicate) {
            return FilteredRange<Range, Predicate>(std::forward<Range>(range), std::move(predicate));
        }
        
        template <typename Predicate>
        class Filter : public RangeOperation<Filter<Predicate>> {
        public:
            constexpr explicit Filter(Predicate predicate) : predicate_(std::move(predicate)) {
            }
            
            template <typename Range>
            constexpr auto operator()(Range &&range) const {
                return FilteredRange<Range, Predicate>(std::forward<Range>(range), predicate_);
            }
            
        private:
            Predicate predicate_;
        };
        
        template <typename Predicate>
        auto filtered(Predicate predicate) {
            return Filter<Predicate>(std::move(predicate));
        }
    }
    
    using filtered_detail::filter;
    using filtered_detail::filtered;
}

#endif
