#ifndef ufo_range_IteratorFacade
#define ufo_range_IteratorFacade

#include <type_traits>
#include <iterator>

namespace ufo {
    struct IteratorAccess {
        template <typename Iterator>
        static constexpr decltype(auto) dereference(const Iterator &iterator) {
            return iterator.dereference();
        }
        
        template <typename Iterator>
        static constexpr void increment(Iterator &iterator) {
            iterator.increment();
        }
        
        template <typename Iterator>
        static constexpr void decrement(Iterator &iterator) {
            iterator.decrement();
        }
        
        template <typename Iterator, typename Difference>
        static constexpr void advance(Iterator &iterator, const Difference &difference) {
            iterator.advance(difference);
        }
        
        template <typename Iterator, typename Difference>
        static constexpr decltype(auto) advanced(const Iterator &iterator, const Difference &difference) {
            return iterator.advanced(iterator, difference);
        }
        
        template <typename LHS, typename RHS>
        static constexpr decltype(auto) distance(const LHS &lhs, const RHS &rhs) {
            return lhs.distance(rhs);
        }
        
        template <typename LHS, typename RHS>
        static constexpr bool equal(const LHS &lhs, const RHS &rhs) {
            return lhs.equal(rhs);
        }
        
        template <typename LHS, typename RHS>
        static constexpr bool less_than(const LHS &lhs, const RHS &rhs) {
            return lhs.less_than(rhs);
        }
    };
    
    template <typename Derived, typename Types>
    class IteratorFacade : public std::iterator<typename Types::iterator_category, typename Types::value_type, typename Types::difference_type, typename Types::pointer, typename Types::reference> {
    protected:
        constexpr IteratorFacade() noexcept = default;
        
        ~IteratorFacade() = default;
        
        constexpr IteratorFacade(const IteratorFacade &) noexcept = default;
        
        constexpr IteratorFacade(IteratorFacade &&) noexcept = default;
        
        constexpr IteratorFacade &operator=(const IteratorFacade &) noexcept = default;
        
        constexpr IteratorFacade &operator=(IteratorFacade &&) noexcept = default;
        
        constexpr Derived &derived() noexcept {
            return *static_cast<Derived*>(this);
        }
        
        constexpr const Derived &derived() const noexcept {
            return *static_cast<const Derived*>(this);
        }
        
    public:
        constexpr decltype(auto) operator*() const {
            return IteratorAccess::dereference(derived());
        }
        
        constexpr decltype(auto) operator->() const noexcept {
            return &*derived();
        }
        
        constexpr decltype(auto) operator[](const typename Types::difference_type &difference) {
            return *IteratorAccess::advanced(derived(), difference);
        }
        
        constexpr auto &operator++() {
            IteratorAccess::increment(derived());
            return derived();
        }
        
        constexpr auto operator++(int) {
            auto result = derived();
            IteratorAccess::increment(derived());
            return result;
        }
        
        constexpr auto &operator--() {
            IteratorAccess::decrement(derived());
            return derived();
        }
        
        constexpr auto operator--(int) {
            auto result = derived();
            IteratorAccess::decrement(derived());
            return result;
        }
        
        constexpr auto &operator+=(const typename Types::difference_type &difference) {
            IteratorAccess::advance(derived(), difference);
            return derived();
        }
        
        constexpr auto &operator-=(const typename Types::difference_type &difference) {
            IteratorAccess::advance(derived(), -difference);
            return derived();
        }
        
    private:
        friend constexpr auto operator+(const Derived &lhs, const typename Types::difference_type &rhs) {
            return IteratorAccess::advanced(lhs, rhs);
        }
        
        friend constexpr auto operator+(const typename Types::difference_type &lhs, const Derived &rhs) {
            return rhs + lhs;
        }
        
        friend constexpr auto operator-(const Derived &lhs, const typename Types::difference_type &rhs) {
            return IteratorAccess::advanced(lhs, -rhs);
        }
        
        friend constexpr auto operator-(const Derived &lhs, const Derived &rhs) {
            return IteratorAccess::distance(lhs, rhs);
        }
        
        friend constexpr bool operator==(const Derived &lhs, const Derived &rhs) {
            return IteratorAccess::equal(lhs, rhs);
        }
        
        friend constexpr bool operator!=(const Derived &lhs, const Derived &rhs) {
            return !(lhs == rhs);
        }
        
        friend constexpr bool operator<(const Derived &lhs, const Derived &rhs) {
            return IteratorAccess::less_than(lhs, rhs);
        }
        
        friend constexpr bool operator<=(const Derived &lhs, const Derived &rhs) {
            return lhs < rhs || lhs == rhs;
        }
        
        friend constexpr bool operator>(const Derived &lhs, const Derived &rhs) {
            return rhs < lhs;
        }
        
        friend constexpr bool operator>=(const Derived &lhs, const Derived &rhs) {
            return rhs <= lhs;
        }
    };
}

#endif
