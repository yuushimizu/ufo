#ifndef ufo_sequence_flatten
#define ufo_sequence_flatten

#include <type_traits>
#include "sequence.hpp"
#include "sequence_operator.hpp"
#include "container_wrapper.hpp"
#include "../option.hpp"

namespace ufo {
    template <typename Sequence>
    class Flattened : public sequence {
    private:
        Sequence sequence_;
        
        constexpr static auto next_inner(Sequence &sequence) {
            return sequence.next().map([](auto &&inner) -> decltype(auto) {return container_wrapper(std::forward<decltype(inner)>(inner));});
        }
        
        decltype(next_inner(std::declval<Sequence &>())) current_ {};
        
    public:
        constexpr Flattened(Sequence sequence) noexcept : sequence_(std::move(sequence)) {
        }
        
        constexpr auto next() -> decltype(current_->next()) {
            if (!current_) current_ = next_inner(sequence_);
            while (current_) {
                if (auto value = current_->next()) return value;
                current_ = next_inner(sequence_);
            }
            return nullopt;
        }
    };
    
    constexpr const auto flatten = sequence_operator([](auto sequence) {
        return Flattened<decltype(sequence)>(std::move(sequence));
    });

}

#endif

