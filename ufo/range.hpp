#ifndef ufo_range
#define ufo_range

#include <algorithm>
#include <experimental/optional>
#include "iterator.hpp"
#include "OptionalReference.hpp"

namespace ufo {
    template <typename Range, typename F>
    constexpr auto find_iterator(Range &&range, F &&predicate) {
        return std::find_if(adl_begin(range), adl_end(range), std::forward<F>(predicate));
    }
    
    template <typename Range, typename F>
    constexpr bool contains(const Range &range, F &&predicate) {
        return find_iterator(range, std::forward<F>(predicate)) != adl_end(range);
    }
    
    template <typename Range, typename Iterator>
    constexpr auto iterator_value_optional(Range &&range, Iterator &&iterator) -> std::experimental::optional<iterator_value_type_t<Iterator>> {
        if (iterator == adl_end(range)) return std::experimental::nullopt;
        return *iterator;
    }
    
    template <typename Range, typename Iterator>
    constexpr auto iterator_reference_optional(Range &&range, Iterator &&iterator) -> OptionalReference<std::remove_reference_t<decltype(*iterator)>> {
        if (iterator == adl_end(range)) return std::experimental::nullopt;
        return *iterator;
    }

    template <typename Range, typename F>
    constexpr auto find(Range &&range, F &&predicate) {
        return iterator_value_optional(range, find_iterator(range, std::forward<F>(predicate)));
    }
    
    template <typename Range, typename F>
    constexpr auto find_reference(Range &&range, F &&predicate) {
        return iterator_reference_optional(range, find_iterator(range, std::forward<F>(predicate)));
    }
    
    template <typename Container, typename F>
    void remove(Container &container, F &&predicate) {
        container.erase(std::remove_if(adl_begin(container), adl_end(container), std::forward<F>(predicate)), adl_end(container));
    }
}

#endif
