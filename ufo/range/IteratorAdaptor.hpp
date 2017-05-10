#ifndef ufo_range_IteratorAdaptor
#define ufo_range_IteratorAdaptor

#include <utility>
#include "../iterator.hpp"
#include "IteratorFacade.hpp"

namespace ufo {
    template <typename Base>
    struct IteratorAdaptorTypes {
        using iterator_category = iterator_category_t<Base>;
        using value_type = iterator_value_type_t<Base>;
        using difference_type = iterator_difference_type_t<Base>;
        using pointer = iterator_pointer_t<Base>;
        using reference = iterator_reference_t<Base>;
    };
    
    template <typename Derived, typename Base, typename Types>
    class IteratorAdaptor : public IteratorFacade<Derived, Types> {
    protected:
        constexpr IteratorAdaptor() = default;
        
        constexpr IteratorAdaptor(Base base) : base_(std::move(base)) {
        }
        
        ~IteratorAdaptor() = default;
        
        constexpr IteratorAdaptor(const IteratorAdaptor &) = default;
        
        constexpr IteratorAdaptor(IteratorAdaptor &&) noexcept = default;
        
        constexpr IteratorAdaptor &operator=(const IteratorAdaptor &) = default;
        
        constexpr IteratorAdaptor &operator=(IteratorAdaptor &&) = default;
        
        constexpr const Base &base() const {
            return base_;
        }
        
        constexpr Base &base() {
            return base_;
        }
        
        constexpr decltype(auto) dereference() const {
            return *base_;
        }
        
        constexpr void increment() {
            ++base_;
        }
        
        constexpr void decrement() {
            --base_;
        }
        
        constexpr void advance(const iterator_difference_type_t<Base> &difference) {
            base_ += difference;
        }
        
        constexpr auto distance(const Derived &rhs) const {
            return base_ - rhs.base_;
        }
        
        constexpr bool equal(const Derived &rhs) const {
            return base_ == rhs.base_;
        }
        
        constexpr bool less_than(const Derived &rhs) const {
            return base_ < rhs.base_;
        }
        
    private:
        Base base_;
    };
}

#endif
