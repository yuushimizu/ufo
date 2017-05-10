#ifndef ufo_range
#define ufo_range

#include <algorithm>
#include <experimental/optional>
#include "iterator.hpp"
#include "OptionalReference.hpp"

namespace ufo {
    template <typename Container, typename T>
    void remove(Container &container, const T &element) {
        container.erase(std::remove(adl_begin(container), adl_end(container), element), adl_end(container));
    }
    
    template <typename Container, typename UnaryFunction>
    void remove_if(Container &container, UnaryFunction &&predicate) {
        container.erase(std::remove_if(adl_begin(container), adl_end(container), std::forward<UnaryFunction>(predicate)), adl_end(container));
    }
    
    template <typename Range, typename T>
    constexpr bool contains(const Range &range, const T &value) {
        return std::find(adl_begin(range), adl_end(range), value) != adl_end(range);
    }
    
    template <typename Range, typename UnaryFunction>
    constexpr bool contains_if(const Range &range, UnaryFunction &&predicate) {
        return std::find_if(adl_begin(range), adl_end(range), std::forward<UnaryFunction>(predicate)) != adl_end(range);
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

    template <typename Range, typename T>
    constexpr auto find(Range &&range, const T &value) {
        return iterator_value_optional(range, std::find(adl_begin(range), adl_end(range), value));
    }
    
    template <typename Range, typename T>
    constexpr auto find_reference(Range &&range, const T &value) {
        return iterator_reference_optional(range, std::find(adl_begin(range), adl_end(range), value));
    }
    
    template <typename Range, typename Predicate>
    constexpr auto find_if(Range &&range, Predicate &&predicate) {
        return iterator_value_optional(range, std::find_if(adl_begin(range), adl_end(range), std::forward<Predicate>(predicate)));
    }
    
    template <typename Range, typename Predicate>
    constexpr auto find_reference_if(Range &&range, Predicate &&predicate) {
        return iterator_reference_optional(range, std::find_if(adl_begin(range), adl_end(range), std::forward<Predicate>(predicate)));
    }
}

#endif
