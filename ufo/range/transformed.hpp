#ifndef ufo_range_transformed
#define ufo_range_transformed

#include <utility>
#include <type_traits>
#include "iterator.hpp"
#include "IteratorAdaptor.hpp"
#include "RangeAdaptor.hpp"

namespace ufo {
    namespace transformed_detail {
        template <typename Base, typename Transformer>
        struct Types : public IteratorAdaptorTypes<Base> {
            using reference = std::result_of_t<Transformer(iterator_reference_t<Base>)>;
            using value_type = std::decay_t<reference>;
            using pointer = value_type*;
        };
        
        template <typename Base, typename Transformer>
        class TransformIterator;
        
        template <typename Base, typename Transformer>
        using IteratorBase = IteratorAdaptor<TransformIterator<Base, Transformer>, Base, Types<Base, Transformer>>;
        
        template <typename Base, typename Transformer>
        class TransformIterator : public IteratorBase<Base, Transformer> {
        public:
            constexpr TransformIterator() = default;
            
            constexpr TransformIterator(Base base, Transformer transformer) : IteratorBase<Base, Transformer>(std::move(base)), transformer_(std::move(transformer)) {
            }
            
        private:
            Transformer transformer_;
            
            friend IteratorAccess;
            
            constexpr decltype(auto) dereference() const {
                return transformer_(*this->base());
            }
            
            constexpr auto advanced(const iterator_difference_type_t<Base> &difference) const {
                return TransformIterator(this->base() + difference, transformer_);
            }
        };
        
        template <typename Base, typename Transformer>
        class TransformedRange : public RangeAdaptor<TransformedRange<Base, Transformer>, Base> {
        public:
            constexpr TransformedRange(Base &&base, Transformer transformer) : RangeAdaptor<TransformedRange<Base, Transformer>, Base>(std::forward<Base>(base)), transformer_(std::move(transformer)) {
            }
            
        private:
            Transformer transformer_;
            
            friend RangeAdaptorAccess;
            
            template <typename This, typename BaseIterator>
            static constexpr auto make_iterator(This &t, BaseIterator &&base) {
                return TransformIterator<std::decay_t<BaseIterator>, Transformer>(std::forward<BaseIterator>(base), t.transformer_);
            }
        };
        
        template <typename Range, typename Transformer>
        constexpr auto transform(Range &&range, Transformer transformer) {
            return TransformedRange<Range, Transformer>(std::forward<Range>(range), std::move(transformer));
        }
        
        template <typename Transformer>
        class Transformed : public RangeOperation<Transformed<Transformer>> {
        public:
            constexpr explicit Transformed(Transformer transformer) : transformer_(std::move(transformer)) {
            }
            
            template <typename Range>
            constexpr auto operator()(Range &&range) const {
                return transform(std::forward<Range>(range), transformer_);
            }
            
        private:
            Transformer transformer_;
        };
        
        template <typename Transformer>
        constexpr auto transformed(Transformer transformer) {
            return Transformed<Transformer>(std::move(transformer));
        }
    }
    
    using transformed_detail::transform;
    using transformed_detail::transformed;
}

#endif
