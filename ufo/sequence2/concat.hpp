#ifndef ufo_sequence2_concat
#define ufo_sequence2_concat

#include "sequence.hpp"
#include "sequence_operator.hpp"
#include "../box.hpp"
#include <type_traits>

namespace ufo::s2 {
    template <typename Front, typename Back>
    constexpr auto concat(Front &&front, Back &&back) {
        return sequence([front = ensure_sequence(std::forward<Front>(front)), back = ensure_sequence(std::forward<Back>(back))] {
            return [front_iterator = front.begin(), back_iterator = back.begin(), front_is_empty = false]() mutable {
                static_assert(std::is_same_v<decltype(front_iterator.next()), decltype(back_iterator.next())>, "elements of concatenated sequences must be the same type");
                if (!front_is_empty) {
                    if (auto result = front_iterator.next()) return result;
                    front_is_empty = true;
                }
                return back_iterator.next();
            };
        });
    }
    
    template <typename Front, typename Back, typename ... Rest>
    constexpr auto concat(Front &&front, Back &&back, Rest && ... rest) {
        return concat(concat(std::forward<Front>(front), std::forward<Back>(back)), std::forward<Rest>(rest) ...);
    }
}

#endif
