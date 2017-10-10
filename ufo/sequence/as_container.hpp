#ifndef ufo_sequence_as_container
#define ufo_sequence_as_container

#include "sequence_operator.hpp"
#include "foreach.hpp"

namespace ufo {
    template <typename Sequence>
    class AsContainer {
    private:
        Sequence sequence_;
        
    public:
        constexpr AsContainer(Sequence sequence) noexcept : sequence_(std::move(sequence)) {
        }
        
        template <typename Container>
        operator Container() && {
            Container result {};
            std::move(sequence_) | foreach([&result](auto &&v) {
                result.push_back(std::forward<decltype(v)>(v));
            });
            return result;
        }
    };
    
    constexpr const auto as_container = sequence_operator([](auto sequence) constexpr noexcept {
        return AsContainer<decltype(sequence)>(std::move(sequence));
    });
}

#endif
