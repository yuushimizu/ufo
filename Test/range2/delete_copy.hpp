#ifndef ufo_test_range2_delete_copy
#define ufo_test_range2_delete_copy

#include "ufo/range2/range_operator.hpp"

namespace ufo::test {
    template <typename Range>
    class DeleteCopy : public range {
    public:
        constexpr DeleteCopy(Range range) : range_(std::move(range)) {
        }
        
        ~DeleteCopy() = default;
        
        DeleteCopy(const DeleteCopy &) = delete;
        
        DeleteCopy(DeleteCopy &&) = default;
        
        DeleteCopy &operator=(const DeleteCopy &) = delete;
        
        DeleteCopy &operator=(DeleteCopy &&) = default;
        
        constexpr decltype(auto) first() const {
            return range_.first();
        }
        
        constexpr void pop() {
            range_.pop();
        }
        
        constexpr bool empty() const {
            return range_.empty();
        }
        
    private:
        Range range_;
    };
    
    constexpr const auto delete_copy = range_operator([](auto range) constexpr {
        return DeleteCopy<decltype(range)>(std::move(range));
    });
    
    template <typename F>
    class DeleteFunctionCopy {
    public:
        constexpr DeleteFunctionCopy(F f) : f_(std::move(f)) {
        }
        
        ~DeleteFunctionCopy() = default;
        
        DeleteFunctionCopy(const DeleteFunctionCopy &) = delete;
        
        DeleteFunctionCopy(DeleteFunctionCopy &&) = default;
        
        DeleteFunctionCopy &operator=(const DeleteFunctionCopy &) = delete;
        
        DeleteFunctionCopy &operator=(DeleteFunctionCopy &&) = default;
        
        template <typename ... Args>
        constexpr decltype(auto) operator()(Args && ... args) const & {
            return f_(std::forward<Args>(args) ...);
        }
        
        template <typename ... Args>
        constexpr decltype(auto) operator()(Args && ... args) & {
            return f_(std::forward<Args>(args) ...);
        }
        
        template <typename ... Args>
        constexpr decltype(auto) operator()(Args && ... args) && {
            return std::move(f_)(std::forward<Args>(args) ...);
        }
        
    private:
        F f_;
    };
    
    template <typename F>
    constexpr auto delete_function_copy(F &&f) {
        return DeleteFunctionCopy<F>(std::forward<F>(f));
    }
}

#endif
