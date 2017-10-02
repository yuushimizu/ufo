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
}

#endif
