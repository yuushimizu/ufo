#ifndef ufo_range_iterate
#define ufo_range_iterate

#include <utility>
#include <iterator>
#include "IteratorFacade.hpp"
#include "RangeFacade.hpp"

namespace ufo {
    namespace iterate_detail {
        template <typename ValueType>
        struct Types {
            using iterator_category = std::input_iterator_tag;
            using reference = ValueType;
            using value_type = ValueType;
            using difference_type = std::ptrdiff_t;
            using pointer = value_type*;
        };
        
        template <typename ValueType, typename Transformer>
        class IterateIterator : public IteratorFacade<IterateIterator<ValueType, Transformer>, Types<ValueType>> {
        public:
            constexpr IterateIterator() = default;
            
            constexpr IterateIterator(ValueType initial_value, Transformer transformer, bool end) : value_(std::move(initial_value)), transformer_(std::move(transformer)), end_(end) {
            }
            
        private:
            ValueType value_;
            Transformer transformer_;
            bool end_;
            
            friend IteratorAccess;
            
            constexpr ValueType dereference() const noexcept {
                return value_;
            }
            
            constexpr void increment() {
                value_ = transformer_(value_);
            }
            
            constexpr bool equal(const IterateIterator &rhs) const noexcept {
                return end_ == rhs.end_;
            }
        };
        
        template <typename ValueType, typename Transformer>
        class IterateRange : public RangeFacade<IterateRange<ValueType, Transformer>> {
        public:
            constexpr IterateRange(ValueType initial_value, Transformer transformer) : initial_value_(std::move(initial_value)), transformer_(std::move(transformer)) {
            }
            
        private:
            ValueType initial_value_;
            Transformer transformer_;
            
            friend RangeAccess;
            
            static constexpr auto make_begin(const IterateRange &t) {
                return IterateIterator<ValueType, Transformer>(t.initial_value_, t.transformer_, false);
            }
            
            static constexpr auto make_end(const IterateRange &t) {
                return IterateIterator<ValueType, Transformer>(t.initial_value_, t.transformer_, true);
            }
        };
        
        template <typename ValueType, typename Transformer>
        constexpr auto iterate(ValueType initial_value, Transformer transformer) {
            return IterateRange<ValueType, Transformer>(std::move(initial_value), std::move(transformer));
        }
    }
    
    using iterate_detail::iterate;
}

#endif
