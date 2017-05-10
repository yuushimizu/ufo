#ifndef ufo_range_unindexed
#define ufo_range_unindexed

#include <utility>
#include "RangeAdaptor.hpp"
#include "transformed.hpp"

namespace ufo {
    namespace unindexed_detail {
        class Unindexed : public RangeOperation<Unindexed> {
        public:
            template <typename Range>
            constexpr auto operator()(Range &&range) const {
                return std::forward<Range>(range) | transformed([](auto &&indexed_entry) -> decltype(auto) {return indexed_entry.value;});
            }
        };
        
        constexpr auto unindexed() {
            return Unindexed {};
        }
    }
    
    using unindexed_detail::unindexed;
}

#endif
