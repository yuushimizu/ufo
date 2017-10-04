#ifndef ufo_sequence_container_wrapper
#define ufo_sequence_container_wrapper

#include <functional>
#include "sequence.hpp"
#include "../iterator.hpp"
#include "../type_traits.hpp"

namespace ufo {
    template <typename Container>
    class ContainerWrapper : public sequence {
    public:
        constexpr ContainerWrapper(Container &&container) : container_(std::move(container)), iterator_(adl_begin(container_)) {
        }
        
        ~ContainerWrapper() = default;
        
        constexpr ContainerWrapper(const ContainerWrapper &other) : container_(other.container_), iterator_(adl_begin(container_) + (other.iterator_ - adl_begin(other.container_))) {
        }
        
        constexpr ContainerWrapper(ContainerWrapper &&other) {
            auto diff = other.iterator_ - adl_begin(other.container_);
            container_ = std::move(other.container_);
            iterator_ = adl_begin(container_) + diff;
        }
        
        constexpr ContainerWrapper &operator=(const ContainerWrapper &other) {
            container_ = other.container_;
            iterator_ = adl_begin(container_) + (other.iterator_ - adl_begin(other.container_));
            return *this;
        }
        
        constexpr ContainerWrapper &operator=(ContainerWrapper &&other) {
            auto diff = other.iterator_ - adl_begin(other.container_);
            container_ = std::move(other.container_);
            iterator_ = adl_begin(container_) + diff;
            return *this;
        }
        
        constexpr auto first() const {
            return *iterator_;
        }
        
        constexpr void pop() {
            ++iterator_;
        }
        
        constexpr bool empty() const {
            return iterator_ == adl_end(container_);
        }
        
    private:
        Container container_;
        decltype(adl_begin(container_)) iterator_;
    };
    
    template <typename Container>
    class ContainerWrapper<Container &> : public sequence {
    public:
        constexpr ContainerWrapper(Container &container) : container_(container), iterator_(adl_begin(container)) {
        }
        
        constexpr decltype(auto) first() const {
            return *iterator_;
        }
        
        constexpr void pop() {
            ++iterator_;
        }
        
        constexpr bool empty() const {
            return iterator_ == adl_end(container_.get());
        }
        
    private:
        std::reference_wrapper<Container> container_;
        decltype(adl_begin(container_.get())) iterator_;
    };
    
    template <typename Container, enable_if_t<!is_sequence_v<Container>> = nullptr>
    constexpr auto container_wrapper(Container &&container) {
        return ContainerWrapper<Container>(std::forward<Container>(container));
    }
    
    template <typename Sequence, enable_if_t<is_sequence_v<Sequence>> = nullptr>
    constexpr auto container_wrapper(Sequence &&container) {
        return std::forward<Sequence>(container);
    }
}

#endif
