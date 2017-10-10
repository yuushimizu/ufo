#ifndef ufo_sequence_concat
#define ufo_sequence_concat

#include <type_traits>
#include "sequence.hpp"
#include "sequence_operator.hpp"

namespace ufo {
    template <typename Front, typename Back>
    class Concatenated : public sequence {
        static_assert(std::is_same_v<sequence_option_t<Front>, sequence_option_t<Back>>);
        
    private:
        Front front_;
        Back back_;
        bool front_is_empty_ = false;
        
    public:
        constexpr Concatenated(Front front, Back back) noexcept : front_(std::move(front)), back_(std::move(back)) {
        }
        
        constexpr auto next() -> sequence_option_t<Front> {
            if (!front_is_empty_) {
                if (auto value = front_.next()) return value;
                front_is_empty_ = true;
            }
            return back_.next();
        }
    };
    
    template <typename Sequence>
    constexpr const auto concatenated(Sequence sequence) noexcept {
        return std::move(sequence);
    }
    
    template <typename Front, typename Back>
    constexpr const auto concatenated(Front front, Back back) noexcept {
        return Concatenated<Front, Back>(std::move(front), std::move(back));
    }
    
    template <typename Front, typename ... Rest>
    constexpr const auto concatenated(Front front, Rest ... rest) noexcept {
        auto back = concatenated(std::move(rest) ...);
        return Concatenated<Front, decltype(back)>(std::move(front), std::move(back));
    }
    
    constexpr const auto concat = sequence_operator([](auto front, auto ... rest) {
        return concatenated(std::move(front), std::move(rest) ...);
    });
}

#endif
