#ifndef ufo_sequence_as_container
#define ufo_sequence_as_container

#include "sequence_operator.hpp"
#include "foreach.hpp"

namespace ufo {
    template <typename Sequence>
    class AsContainer {
    public:
        constexpr AsContainer(Sequence sequence) : sequence_(std::move(sequence)) {
        }
        
        template <typename Container>
        operator Container() && {
            Container result {};
            std::move(sequence_) | foreach([&result](auto &&v) {
                result.push_back(std::forward<decltype(v)>(v));
            });
            return result;
        }
        
    private:
        Sequence sequence_;
    };
    
    constexpr const auto as_container = sequence_operator([](auto sequence) {
        return AsContainer<decltype(sequence)>(std::move(sequence));
    });
}

#endif
