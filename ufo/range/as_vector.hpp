#ifndef ufo_range_as_vector
#define ufo_range_as_vector

#include <utility>
#include <vector>
#include "iterator.hpp"
#include "RangeAdaptor.hpp"
#include "as_container.hpp"

namespace ufo {
    namespace as_vector_detail {
        class AsVector : public RangeOperation<AsVector> {
        public:
            template <typename Range>
            constexpr auto operator()(Range &&range) const -> std::vector<iterator_value_type_t<decltype(adl_begin(range))>> {
                return std::forward<Range>(range) | as_container();
            }
        };
        
        constexpr auto as_vector() {
            return AsVector {};
        }
    }
    
    using as_vector_detail::as_vector;
}

#endif
