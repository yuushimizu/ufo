#ifndef ufo_sequence_as_container
#define ufo_sequence_as_container

#include "sequence_operator.hpp"
#include "foreach.hpp"
#include "../type_traits.hpp"

namespace ufo {
    template <typename Sequence>
    class AsContainer {
    private:
        Sequence sequence_;
        
    public:
        explicit constexpr AsContainer(const Sequence &sequence) : sequence_(sequence) {
        }
        
        explicit constexpr AsContainer(Sequence &&sequence) noexcept : sequence_(std::move(sequence)) {
        }
        
        template <typename Container>
        operator Container() && {
            Container result {};
            std::forward<Sequence>(sequence_) | foreach([&result](auto &&v) {
                result.push_back(std::forward<decltype(v)>(v));
            });
            return result;
        }
    };
    
    template <typename Sequence>
    constexpr auto make_as_container(Sequence &&sequence) {
        return AsContainer<Sequence>(std::forward<Sequence>(sequence));
    }
    
    constexpr auto as_container() noexcept {
        return sequence_operator([](auto &&sequence) constexpr noexcept {
            return make_as_container(std::forward<decltype(sequence)>(sequence));
        });
    }
}

#endif
