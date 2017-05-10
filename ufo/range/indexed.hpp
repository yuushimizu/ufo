#ifndef ufo_range_indexed
#define ufo_range_indexed

#include <utility>
#include "RangeAdaptor.hpp"
#include "arithmetic_progression.hpp"
#include "combine.hpp"

namespace ufo {
    namespace indexed_detail {
        template <typename Index, typename Value>
        class IndexedEntry {
        public:
            Index index;
            Value value;
            
            constexpr IndexedEntry(Index index, Value &&value)
            : index(std::move(index))
            , value(std::forward<Value>(value))
            {
            }
        };
        
        template <typename Index, typename Value>
        constexpr auto indexed_entry(Index index, Value &&value) {
            return IndexedEntry<Index, Value>(std::move(index), std::forward<Value>(value));
        }
        
        class Indexed : public RangeOperation<Indexed> {
        public:
            template <typename Range>
            constexpr auto operator()(Range &&range) const {
                return combine(arithmetic_progression<std::size_t>(), std::forward<Range>(range), [](auto &&index, auto &&value) {return indexed_entry(std::forward<decltype(index)>(index), std::forward<decltype(value)>(value));});
            }
        };
        
        constexpr auto indexed() {
            return Indexed {};
        }
    }
    
    using indexed_detail::indexed_entry;
    using indexed_detail::indexed;
}

#endif
