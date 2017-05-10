#ifndef ufo_range_taken_while
#define ufo_range_taken_while

#include <utility>
#include <type_traits>
#include <iterator>
#include "iterator.hpp"
#include "IteratorAdaptor.hpp"
#include "RangeAdaptor.hpp"

namespace ufo {
    namespace taken_while_detail {
        template <typename Base>
        struct Types : public IteratorAdaptorTypes<Base> {
            using iterator_category = std::forward_iterator_tag;
        };
        
        template <typename Base, typename Predicate>
        class TakeWhileIterator;
        
        template <typename Base, typename Predicate>
        using IteratorBase = IteratorAdaptor<TakeWhileIterator<Base, Predicate>, Base, Types<Base>>;
        
        template <typename Base, typename Predicate>
        class TakeWhileIterator : public IteratorBase<Base, Predicate> {
        private:
            constexpr bool is_end() const {
                return this->base() == base_end_ || !predicate_(this->dereference());
            }
            
        public:
            constexpr TakeWhileIterator() = default;

            constexpr TakeWhileIterator(Base base, Base base_end, Predicate predicate) : IteratorBase<Base, Predicate>(std::move(base)), base_end_(std::move(base_end)), predicate_(std::move(predicate)), end_(is_end()) {
            }
            
        private:
            Base base_end_;
            Predicate predicate_;
            bool end_;
            
            friend IteratorAccess;
            
            constexpr void increment() {
                IteratorBase<Base, Predicate>::increment();
                if (is_end()) end_ = true;
            }
            
            constexpr bool equal(const TakeWhileIterator &rhs) const {
                return end_ ? rhs.end_ : IteratorBase<Base, Predicate>::equal(rhs);
            }
        };
        
        template <typename Base, typename Predicate>
        class TakenWhileRange : public RangeAdaptor<TakenWhileRange<Base, Predicate>, Base> {
        public:
            constexpr TakenWhileRange(Base &&base, Predicate predicate) : RangeAdaptor<TakenWhileRange<Base, Predicate>, Base>(std::forward<Base>(base)), predicate_(std::move(predicate)) {
            }
            
        private:
            Predicate predicate_;
            
            friend RangeAdaptorAccess;
            
            template <typename This, typename BaseIterator>
            static constexpr auto make_iterator(This &t, BaseIterator &&base) {
                return TakeWhileIterator<std::decay_t<BaseIterator>, Predicate>(std::forward<BaseIterator>(base), adl_end(t.base()), t.predicate_);
            }
        };
        
        template <typename Range, typename Predicate>
        constexpr auto take_while(Range &&range, Predicate predicate) {
            return TakenWhileRange<Range, Predicate>(std::forward<Range>(range), std::move(predicate));
        }
        
        template <typename Predicate>
        class TakenWhile : public RangeOperation<TakenWhile<Predicate>> {
        public:
            constexpr explicit TakenWhile(Predicate predicate) : predicate_(std::move(predicate)) {
            }
            
            template <typename Range>
            constexpr auto operator()(Range &&range) const {
                return take_while(std::forward<Range>(range), predicate_);
            }
            
        private:
            Predicate predicate_;
        };
        
        template <typename Predicate>
        auto taken_while(Predicate predicate) {
            return TakenWhile<Predicate>(std::move(predicate));
        }
    }
    
    using taken_while_detail::take_while;
    using taken_while_detail::taken_while;
}

#endif
