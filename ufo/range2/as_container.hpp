#ifndef ufo_range2_as_container
#define ufo_range2_as_container

#include "range_operator.hpp"
#include "foreach.hpp"

namespace ufo {
    template <typename Range>
    class AsContainer {
    public:
        constexpr AsContainer(Range range) : range_(std::move(range)) {
        }
        
        template <typename Container>
        operator Container() && {
            Container result {};
            std::move(range_) | foreach([&result](auto &&v) {
                result.push_back(std::forward<decltype(v)>(v));
            });
            return result;
        }
        
    private:
        Range range_;
    };
    
    constexpr const auto as_container = range_operator([](auto range) {
        return AsContainer<decltype(range)>(std::move(range));
    });
}

#endif
