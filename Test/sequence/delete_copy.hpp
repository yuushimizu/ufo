#ifndef ufo_test_sequence_delete_copy
#define ufo_test_sequence_delete_copy

#include "ufo/sequence/sequence_operator.hpp"

namespace ufo::test {
    template <typename Sequence>
    class DeleteCopy : public sequence {
    public:
        constexpr DeleteCopy(Sequence sequence) : sequence_(std::move(sequence)) {
        }
        
        ~DeleteCopy() = default;
        
        DeleteCopy(const DeleteCopy &) = delete;
        
        DeleteCopy(DeleteCopy &&) = default;
        
        DeleteCopy &operator=(const DeleteCopy &) = delete;
        
        DeleteCopy &operator=(DeleteCopy &&) = default;
        
        constexpr decltype(auto) next() {
            return sequence_.next();
        }
        
    private:
        Sequence sequence_;
    };
    
    constexpr const auto delete_copy = sequence_operator([](auto sequence) constexpr {
        return DeleteCopy<decltype(sequence)>(std::move(sequence));
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
