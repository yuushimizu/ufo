#ifndef ufo_sequence2_to_container
#define ufo_sequence2_to_container

#include "sequence_operator.hpp"
#include "foreach.hpp"

namespace ufo::s2 {
    namespace to_container_ {
        template <typename Seq>
        class containerized {
        public:
            containerized(Seq seq) : seq_(std::move(seq)) {
            }
            
            template <typename Container>
            operator Container() && {
                auto result = Container {};
                std::move(seq_) | ufo::s2::foreach([&result](auto &&element) {
                    result.push_back(std::forward<decltype(element)>(element));
                });
                return result;
            }
            
        private:
            Seq seq_;
        };
    }
    
    constexpr auto to_container() noexcept {
        return sequence_operator([](auto &&seq) {
            return to_container_::containerized(std::forward<decltype(seq)>(seq));
        });
    }
}

#endif
