#ifndef ufo_sequence_to_container
#define ufo_sequence_to_container

#include "sequence_operator.hpp"
#include "foreach.hpp"
#include "../type_traits.hpp"

namespace ufo {
    template <typename Sequence>
    class ToContainer {
    private:
        Sequence sequence_;
        
    public:
        explicit constexpr ToContainer(const Sequence &sequence) : sequence_(sequence) {
        }
        
        explicit constexpr ToContainer(Sequence &&sequence) noexcept : sequence_(std::move(sequence)) {
        }
        
        template <typename Container>
        operator Container() && {
            auto result = Container {};
            std::forward<Sequence>(sequence_) | foreach([&result](auto &&v) {
                result.push_back(std::forward<decltype(v)>(v));
            });
            return result;
        }
    };
    
    template <typename Sequence>
    constexpr auto make_to_container(Sequence &&sequence) {
        return ToContainer<Sequence>(std::forward<Sequence>(sequence));
    }
    
    constexpr auto to_container() noexcept {
        return sequence_operator([](auto &&sequence) constexpr noexcept {
            return make_to_container(std::forward<decltype(sequence)>(sequence));
        });
    }
}

#endif
