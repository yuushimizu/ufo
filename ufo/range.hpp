#ifndef ufo_range
#define ufo_range

#include <algorithm>
#include "iterator.hpp"
#include "optional.hpp"
#include "optref.hpp"

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
    constexpr auto iterator_optional(Range &&range, Iterator &&iterator) -> optional<iterator_value_type_t<Iterator>> {
        if (iterator == adl_end(range)) return nullopt;
        return *iterator;
    }
    
    template <typename Range, typename Iterator>
    constexpr auto iterator_optref(Range &&range, Iterator &&iterator) -> optref<std::remove_reference_t<decltype(*iterator)>> {
        if (iterator == adl_end(range)) return nullopt;
        return *iterator;
    }

    template <typename Range, typename F>
    constexpr auto find(Range &&range, F &&predicate) {
        return iterator_optional(range, find_iterator(range, std::forward<F>(predicate)));
    }
    
    template <typename Range, typename F>
    constexpr auto find_ref(Range &&range, F &&predicate) {
        return iterator_optref(range, find_iterator(range, std::forward<F>(predicate)));
    }
    
    template <typename Container, typename F>
    void remove(Container &container, F &&predicate) {
        container.erase(std::remove_if(adl_begin(container), adl_end(container), std::forward<F>(predicate)), adl_end(container));
    }
}

#endif
