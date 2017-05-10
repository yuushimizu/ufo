#ifndef ufo_range_taken
#define ufo_range_taken

#include <utility>
#include "RangeAdaptor.hpp"
#include "indexed.hpp"
#include "taken_while.hpp"
#include "unindexed.hpp"

namespace ufo {
    namespace taken_detail {
        class Taken : public RangeOperation<Taken> {
        public:
            constexpr explicit Taken(std::size_t limit) : limit_(limit) {
            }
            
            template <typename Range>
            constexpr auto operator()(Range &&range) const {
                return std::forward<Range>(range) | indexed() | taken_while([limit = limit_](auto &&indexed_entry) {return indexed_entry.index < limit;}) | unindexed();
            }
            
        private:
            std::size_t limit_;
        };
        
        constexpr auto taken(std::size_t limit) {
            return Taken(limit);
        };
    }
    
    using taken_detail::taken;
}

#endif
