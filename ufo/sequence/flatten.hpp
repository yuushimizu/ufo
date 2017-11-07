#ifndef ufo_sequence_flatten
#define ufo_sequence_flatten

#include <type_traits>
#include "sequence_wrapper.hpp"
#include "sequence_operator.hpp"
#include "container_wrapper.hpp"

namespace ufo {
    namespace flatten_detail {
        template <typename Sequence>
        constexpr auto next_inner(Sequence &&sequence) {
            return std::forward<Sequence>(sequence).next().map([](auto &&inner) -> decltype(auto) {return container_wrapper(std::forward<decltype(inner)>(inner));});
        }
    }
    
    constexpr inline const auto flatten = sequence_operator([](auto &&sequence) {
        using inner_option = decltype(flatten_detail::next_inner(std::forward<decltype(sequence)>(sequence)));
        return sequence_wrapper([current = inner_option {}](auto &sequence) constexpr mutable -> decltype(std::declval<inner_option>()->next()) {
            if (!current) current = flatten_detail::next_inner(sequence);
            while (current) {
                if (auto value = current->next()) return value;
                current = flatten_detail::next_inner(sequence);
            }
            return nullopt;
        }, std::forward<decltype(sequence)>(sequence));
    });
}

#endif

