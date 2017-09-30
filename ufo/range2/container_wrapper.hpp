#ifndef ufo_range2_container_wrapper
#define ufo_range2_container_wrapper

#include <functional>
#include "../iterator.hpp"

namespace ufo {
    template <typename Container>
    class ContainerWrapper {
    public:
        constexpr ContainerWrapper(Container &&container) : container_(std::move(container)), iterator_(adl_begin(container_)) {
        }
        
        auto first() const {
            return *iterator_;
        }
        
        auto rest() && {
            ++iterator_;
            return std::move(*this);
        }
        
    private:
        Container container_;
        decltype(adl_begin(container_)) iterator_;
    };
    
    template <typename Container>
    class ContainerWrapper<Container &> {
    public:
        constexpr ContainerWrapper(Container &container) : container_(container), iterator_(adl_begin(container)) {
        }
        
        decltype(auto) first() const {
            return *iterator_;
        }
        
        auto rest() && {
            ++iterator_;
            return std::move(*this);
        }
        
    private:
        std::reference_wrapper<Container> container_;
        decltype(adl_begin(container_.get())) iterator_;
    };
    
    template <typename Container>
    constexpr auto container_wrapper(Container &&container) {
        return ContainerWrapper<Container>(std::forward<Container>(container));
    }
}

#endif
