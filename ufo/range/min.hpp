#ifndef ufo_range_min
#define ufo_range_min

#include <utility>
#include "../optional.hpp"
#include "../iterator.hpp"
#include "RangeAdaptor.hpp"

namespace ufo {
    namespace min_detail {
        template <typename KeyFunction>
        class Min : public RangeOperation<Min<KeyFunction>> {
        public:
            constexpr explicit Min(KeyFunction key_function) : key_function_(std::move(key_function)) {
            }
            
            template <typename Range>
            constexpr auto operator()(Range &&range) -> optional<iterator_value_type_t<decltype(adl_begin(range))>> {
                auto iter = adl_begin(range);
                auto end = adl_end(range);
                if (iter == end) return nullopt;
                auto min_value = *iter;
                auto min_key = key_function_(min_value);
                for (++iter; iter != end; ++iter) {
                    auto value = *iter;
                    auto key = key_function_(value);
                    if (key < min_key) {
                        min_value = std::move(value);
                        min_key = std::move(key);
                    }
                }
                return min_value;
            }
            
        private:
            KeyFunction key_function_;
        };
        
        template <typename KeyFunction>
        constexpr auto min(KeyFunction key_function) {
            return Min<KeyFunction>(std::move(key_function));
        }
    }
    
    using min_detail::min;
}

#endif
