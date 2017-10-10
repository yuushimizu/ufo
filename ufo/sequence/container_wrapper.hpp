#ifndef ufo_sequence_container_wrapper
#define ufo_sequence_container_wrapper

#include <functional>
#include "sequence.hpp"
#include "../iterator.hpp"
#include "../type_traits.hpp"
#include "../option.hpp"

namespace ufo {
    template <typename Container>
    class ContainerWrapper : public sequence {
    private:
        Container container_;
        decltype(adl_begin(container_)) iterator_;
        
    public:
        constexpr ContainerWrapper(Container &&container) noexcept : container_(std::move(container)), iterator_(adl_begin(container_)) {
        }
        
        ~ContainerWrapper() = default;
        
        constexpr ContainerWrapper(const ContainerWrapper &other) : container_(other.container_), iterator_(adl_begin(container_) + (other.iterator_ - adl_begin(other.container_))) {
        }
        
        constexpr ContainerWrapper(ContainerWrapper &&other) noexcept {
            auto diff = other.iterator_ - adl_begin(other.container_);
            container_ = std::move(other.container_);
            iterator_ = adl_begin(container_) + diff;
        }
        
        constexpr ContainerWrapper &operator=(const ContainerWrapper &other) {
            *this = ContainerWrapper(other);
            return *this;
        }
        
        constexpr ContainerWrapper &operator=(ContainerWrapper &&other) noexcept {
            auto diff = other.iterator_ - adl_begin(other.container_);
            container_ = std::move(other.container_);
            iterator_ = adl_begin(container_) + diff;
            return *this;
        }
        
        constexpr auto next() noexcept -> option<std::decay_t<decltype(*iterator_)>> {
            if (iterator_ == adl_end(container_)) return nullopt;
            auto value = std::move(*iterator_);
            ++iterator_;
            return std::move(value);
        }
    };
    
    template <typename Container>
    class ContainerWrapper<Container &> : public sequence {
    private:
        std::reference_wrapper<Container> container_;
        decltype(adl_begin(container_.get())) iterator_;
        
    public:
        constexpr ContainerWrapper(Container &container) noexcept : container_(container), iterator_(adl_begin(container)) {
        }
        
        constexpr auto next() noexcept -> option<decltype(*iterator_)> {
            if (iterator_ == adl_end(container_.get())) return nullopt;
            auto &value = *iterator_;
            ++iterator_;
            return value;
        }
    };
    
    template <typename Container, enable_if_t<!is_sequence_v<Container>> = nullptr>
    constexpr auto container_wrapper(Container &&container) {
        return ContainerWrapper<Container>(std::forward<Container>(container));
    }
    
    template <typename Sequence, enable_if_t<is_sequence_v<Sequence>> = nullptr>
    constexpr decltype(auto) container_wrapper(Sequence &&container) {
        return std::forward<Sequence>(container);
    }
}

#endif
