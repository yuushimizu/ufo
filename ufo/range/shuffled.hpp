#ifndef ufo_range_shuffled
#define ufo_range_shuffled

#include <utility>
#include <type_traits>
#include "iterator.hpp"
#include "RangeAdaptor.hpp"
#include "as_vector.hpp"

namespace ufo {
    namespace shuffled_detail {
        template <typename RandomNumberGenerator>
        class Shuffled : public RangeOperation<Shuffled<RandomNumberGenerator>> {
        public:
            constexpr explicit Shuffled(RandomNumberGenerator generator) : generator_(std::forward<RandomNumberGenerator>(generator)) {
            }
            
            template <typename Range>
            constexpr auto operator()(Range &&range) {
                auto result = std::forward<Range>(range) | as_vector();
                std::shuffle(adl_begin(result), adl_end(result), generator_);
                return result;
            };
            
        private:
            RandomNumberGenerator generator_;
        };
        
        template <typename RandomNumberGenerator>
        constexpr auto shuffled(RandomNumberGenerator &generator) {
            return Shuffled<RandomNumberGenerator>(generator);
        }
    }
    
    using shuffled_detail::shuffled;
}

#endif
