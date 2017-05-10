#ifndef ufo_range_irange
#define ufo_range_irange

#include <utility>
#include <type_traits>
#include "arithmetic_progression.hpp"
#include "taken_while.hpp"

namespace ufo {
    template <typename Begin, typename End, typename Step>
    constexpr auto irange(Begin begin, End end, Step step) {
        return arithmetic_progression(std::move(begin), std::move(step)) | taken_while([end = std::move(end)](auto &&n) {return n < end;});
    }
    
    template <typename Begin, typename End>
    constexpr auto irange(Begin &&begin, End &&end) {
        return irange(std::forward<Begin>(begin), std::forward<End>(end), 1);
    }
    
    template <typename T>
    constexpr auto irange(T &&end) {
        return irange(std::decay_t<T> {}, std::forward<T>(end));
    }
}
#endif
