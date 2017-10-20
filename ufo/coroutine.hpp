#ifndef ufo_coroutine
#define ufo_coroutine

#include <forward_list>
#include "option.hpp"
#include "type_traits.hpp"
#include "scope_exit.hpp"
#include "container.hpp"

namespace ufo {
    template <typename>
    class coroutine;
    
    template <typename R, typename ... Args>
    class coroutine<R(Args ...)> final {
    private:
        class Part;
        
        class Part final {
        private:
            class Impl {
            public:
                Impl() = default;
                
                virtual ~Impl() = default;
                
                Impl(const Impl &) = delete;
                
                Impl(Impl &&) = delete;
                
                Impl &operator=(const Impl &) = delete;
                
                Impl &operator=(Impl &&) = delete;
                
                virtual R call_and_merge_to(coroutine &parent, Args && ...) = 0;
            };
            
            template <typename F>
            class Normal : public Impl {
            public:
                Normal(F f) : f_(std::move(f)) {
                }
                
                virtual ~Normal() = default;
                
                virtual R call_and_merge_to(coroutine &, Args && ... args) override {
                    return f_(std::forward<Args>(args) ...);
                }
                
            private:
                F f_;
            };
            
            template <typename F>
            class Nested : public Impl {
            public:
                Nested(F f) : f_(std::move(f)) {
                }
                
                virtual ~Nested() = default;
                
                virtual R call_and_merge_to(coroutine &parent, Args && ... args) override {
                    return f_(args ...).call_and_merge_to(parent, std::forward<Args>(args) ...);
                }
                
            private:
                F f_;
            };
            
        public:
            Part(std::unique_ptr<Impl> impl) : impl_(std::move(impl)) {
            }
            
            template <typename F, enable_if_t<!std::is_same_v<Part, std::decay_t<F>> && std::is_convertible_v<std::result_of_t<F(Args ...)>, R>> = nullptr>
            explicit Part(F f) : impl_(std::make_unique<Normal<F>>(std::move(f))) {
            }
            
            template <typename F, enable_if_t<!std::is_same_v<Part, std::decay_t<F>> && std::is_convertible_v<std::result_of_t<F(Args ...)>, coroutine<R(Args ...)>>> = nullptr>
            explicit Part(F f) : impl_(std::make_unique<Nested<F>>(std::move(f))) {
            }
            
            ~Part() = default;
            
            Part(const Part &) = delete;
            
            Part(Part &&) = default;
            
            Part &operator=(const Part &) = delete;
            
            Part &operator=(Part &&) = default;
            
            R call_and_merge_to(coroutine &parent, Args && ... args) {
                return (*impl_).call_and_merge_to(parent, std::forward<Args>(args) ...);
            }
            
        private:
            std::unique_ptr<Impl> impl_;
        };
        
        static auto add_parts(std::forward_list<Part> &parts) {
        }
        
        template <typename T, typename ... Rest, enable_if_t<!std::is_same_v<coroutine<R(Args ...)>, std::decay_t<T>>> = nullptr>
        static auto add_parts(std::forward_list<Part> &parts, T &&arg, Rest && ... rest) {
            add_parts(parts, std::forward<Rest>(rest) ...);
            parts.push_front(Part(std::forward<T>(arg)));
        }
        
        template <typename T, typename ... Rest, enable_if_t<std::is_same_v<coroutine<R(Args ...)>, std::decay_t<T>>> = nullptr>
        static auto add_parts(std::forward_list<Part> &parts, T other, Rest && ... rest) {
            add_parts(parts, std::forward<Rest>(rest) ...);
            parts.splice_after(parts.before_begin(), std::move(other.parts_));
        }
        
        template <typename ... PartArgs>
        static auto make_parts(PartArgs && ... part_args) {
            std::forward_list<Part> parts {};
            add_parts(parts, std::forward<PartArgs>(part_args) ...);
            return parts;
        }
        
        R call_and_merge_to(coroutine &parent, Args && ... args) && {
            parent.parts_.splice_after(parent.parts_.before_begin(), std::move(parts_));
            return parent(std::forward<Args>(args) ...);
        }
        
    public:
        constexpr coroutine() noexcept = default;
        
        template <typename T, enable_if_t<!std::is_same_v<coroutine<R(Args ...)>, std::decay_t<T>>> = nullptr>
        explicit coroutine(T &&arg) : parts_(make_parts(std::forward<T>(arg))) {
        }
        
        template <typename First, typename Second, typename ... PartArgs>
        explicit coroutine(First &&first, Second &&second, PartArgs && ... part_args) : parts_(make_parts(std::forward<First>(first), std::forward<Second>(second), std::forward<PartArgs>(part_args) ...)) {
        }
        
        ~coroutine() = default;
        
        coroutine(const coroutine &) = delete;
        
        coroutine(coroutine &&) noexcept = default;
        
        coroutine &operator=(const coroutine &) = delete;
        
        coroutine &operator=(coroutine &&) noexcept = default;
        
        R operator()(Args ... args) {
            auto part = std::move(parts_.front());
            parts_.pop_front();
            return part.call_and_merge_to(*this, std::forward<Args>(args) ...);
        }
        
        bool is_finished() const noexcept {
            return parts_.empty();
        }
        
    private:
        std::forward_list<Part> parts_;
    };
}

#endif
