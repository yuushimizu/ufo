#ifndef ufo_container
#define ufo_container

#include "iterator.hpp"
#include "option.hpp"

namespace ufo {
    template <typename Container, typename F>
    void remove(Container &container, F &&predicate) {
        container.erase(std::remove_if(adl_begin(container), adl_end(container), std::forward<F>(predicate)), adl_end(container));
    }
    
    template <typename Map, typename Key>
    constexpr auto get(Map &map, const Key &key) -> option<decltype(map.at(key))> {
        if (map.find(key) == adl_end(map)) return nullopt;
        return std::forward<Map>(map).at(key);
    }
    
    template <typename Map, typename Key>
    constexpr auto get(Map &&map, const Key &key) -> option<std::remove_reference_t<decltype(map.at(key))>> {
        if (map.find(key) == adl_end(map)) return nullptr;
        return std::move(map.at(key));
    }
}

#endif