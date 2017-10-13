#ifndef ufo_container
#define ufo_container

#include <algorithm>
#include "iterator.hpp"
#include "option.hpp"

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
    constexpr auto iterator_optional(Range &&range, Iterator iterator) -> option<iterator_value_type_t<Iterator>> {
        if (iterator == adl_end(range)) return nullopt;
        return *iterator;
    }
    
    template <typename Range, typename Iterator>
    constexpr auto iterator_optref(Range &&range, Iterator iterator) -> option<std::remove_reference_t<decltype(*iterator)> &> {
        if (iterator == adl_end(range)) return nullopt;
        return *iterator;
    }

    template <typename Range, typename F>
    constexpr auto find(const Range &range, F &&predicate) {
        return iterator_optional(range, find_iterator(range, std::forward<F>(predicate)));
    }
    
    template <typename Range, typename F>
    auto find_ref(Range &&range, F &&predicate) {
        return iterator_optref(range, find_iterator(range, std::forward<F>(predicate)));
    }
    
    template <typename Container, typename F>
    void remove(Container &container, F &&predicate) {
        container.erase(std::remove_if(adl_begin(container), adl_end(container), std::forward<F>(predicate)), adl_end(container));
    }
    
    template <typename Map, typename Key>
    constexpr auto get(const Map &map, const Key &key) -> option<typename Map::mapped_type> {
        if (map.find(key) == adl_end(map)) return nullopt;
        return map.at(key);
    }
    
    template <typename Map, typename Key>
    constexpr auto get_ref(Map &&map, const Key &key) -> option<std::remove_reference_t<decltype(map.at(key))> &> {
        if (map.find(key) == adl_end(map)) return nullopt;
        return map.at(key);
    }
}

#endif
