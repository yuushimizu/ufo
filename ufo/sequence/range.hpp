#ifndef ufo_sequence_range
#define ufo_sequence_range

#include "counting.hpp"
#include "take_while.hpp"

namespace ufo {
    template <typename Begin, typename End, typename Step>
    constexpr auto range(Begin begin, End end, Step step) {
        bool up = step >= 0;
        return counting(std::move(begin), std::move(step)) | take_while([end = std::move(end), up](const auto &n) {
            return up ? n < end : n > end;
        });
    }
    
    template <typename Begin, typename End>
    constexpr auto range(Begin begin, End end) {
        return range(std::move(begin), std::move(end), Begin(1));
    }
    
    template <typename End>
    constexpr auto range(End end) {
        return range(End {}, std::move(end));
    }
}

#endif

