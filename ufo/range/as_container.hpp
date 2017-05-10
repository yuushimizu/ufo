#ifndef ufo_range_as_container
#define ufo_range_as_container

#include <utility>
#include "../iterator.hpp"
#include "RangeAdaptor.hpp"

namespace ufo {
    namespace as_container_detail {
        template <typename Range>
        class AsContainerWrapper {
        public:
            constexpr explicit AsContainerWrapper(Range &&range) : range_(std::forward<Range>(range)) {
            }
            
            template <typename Container>
            constexpr operator Container() const {
                return Container(adl_begin(range_), adl_end(range_));
            }
            
            constexpr operator Range() const& {
                return range_;
            }
            
            constexpr operator Range() && {
                return std::forward<Range>(range_);
            }
            
        private:
            Range range_;
        };
        
        class AsContainer : public RangeOperation<AsContainer> {
        public:
            template <typename Range>
            constexpr auto operator()(Range &&range) const {
                return AsContainerWrapper<Range>(std::forward<Range>(range));
            }
        };
        
        constexpr auto as_container() {
            return AsContainer {};
        }
    }
    
    using as_container_detail::as_container;
}

#endif
