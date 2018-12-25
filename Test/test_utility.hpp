#ifndef ufo_test_test_utility
#define ufo_test_test_utility

namespace ufo::test {
    template <typename T>
    class nocopy {
    public:
        constexpr nocopy(T value) noexcept : value_(std::move(value)) {
        }
        
        constexpr nocopy(const nocopy &) = delete;
        
        constexpr nocopy(nocopy &&) = default;
        
        constexpr nocopy &operator=(const nocopy &) = delete;
        
        constexpr nocopy &operator=(nocopy &&) = default;
        
        ~nocopy() noexcept = default;
        
        template <typename ... Args>
        constexpr decltype(auto) operator()(Args && ... args) const & {
            return value_(std::forward<Args>(args) ...);
        }
        
        template <typename ... Args>
        constexpr decltype(auto) operator()(Args && ... args) & {
            return value_(std::forward<Args>(args) ...);
        }
        
        template <typename ... Args>
        constexpr decltype(auto) operator()(Args && ... args) && {
            return std::move(value_)(std::forward<Args>(args) ...);
        }
    private:
        T value_;
    };
}

#endif
