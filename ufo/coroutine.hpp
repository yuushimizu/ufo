#ifndef ufo_coroutine
#define ufo_coroutine

#include <deque>
#include "option.hpp"
#include "type_traits.hpp"
#include "scope_exit.hpp"

namespace ufo {
    template <typename>
    class coroutine;
    
    template <typename R, typename ... Args>
    class coroutine<R(Args ...)> final {
    private:
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
                
                virtual R operator()(Args && ...) = 0;
                
                bool is_finished() const {
                    return finished_;
                }
                
            protected:
                void finish() {
                    finished_ = true;
                }
                
            private:
                bool finished_ = false;
            };
            
            template <typename F>
            class Normal : public Impl {
            public:
                Normal(F f) : f_(std::move(f)) {
                }
                
                virtual ~Normal() = default;
                
                virtual R operator()(Args && ... args) override {
                    auto finish = scope_exit([this]() {
                        this->finish();
                    });
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
                
                virtual R operator()(Args && ... args) override {
                    if (!called_) {
                        coro_ = f_(args ...);
                        called_ = true;
                    }
                    auto finish = scope_exit([this]() {
                        if (this->coro_->is_finished()) this->finish();
                    });
                    return (*coro_)(std::forward<Args>(args) ...);
                }
                
            private:
                F f_;
                option<coroutine<R(Args ...)>> coro_ = nullopt;
                bool called_ = false;
            };
            
            class Delegate : public Impl {
            public:
                Delegate(coroutine<R(Args ...)> coro) : coro_(std::move(coro)) {
                }
                
                virtual ~Delegate() = default;
                
                virtual R operator()(Args && ... args) override {
                    auto finish = scope_exit([this]() {
                        if (this->coro_.is_finished()) this->finish();
                    });
                    return coro_(std::forward<Args>(args) ...);
                }
                
            private:
                coroutine<R(Args ...)> coro_;
            };
            
        public:
            Part(std::unique_ptr<Impl> impl) : impl_(std::move(impl)) {
            }
            
            template <typename F, enable_if_t<std::is_convertible_v<std::result_of_t<F(Args ...)>, R>> = nullptr>
            explicit Part(F f) : impl_(std::make_unique<Normal<F>>(std::move(f))) {
            }
            
            template <typename F, enable_if_t<std::is_convertible_v<std::result_of_t<F(Args ...)>, coroutine<R(Args ...)>>> = nullptr>
            explicit Part(F f) : impl_(std::make_unique<Nested<F>>(std::move(f))) {
            }
            
            explicit Part(coroutine<R(Args ...)> coro) : impl_(std::make_unique<Delegate>(std::move(coro))) {
            }
            
            ~Part() = default;
            
            Part(const Part &) = delete;
            
            Part(Part &&) = default;
            
            Part &operator=(const Part &) = delete;
            
            Part &operator=(Part &&) = default;
            
            R call(Args ... args) {
                return (*impl_)(std::forward<Args>(args) ...);
            }
            
            bool is_finished() const {
                return impl_->is_finished();
            }
            
        private:
            std::unique_ptr<Impl> impl_;
        };
        
    public:
        coroutine() : parts_ {} {
        }
        
        template <typename F, typename ... Fs>
        explicit coroutine(F f, Fs ... fs) : coroutine(std::move(fs) ...) {
            parts_.push_front(Part(std::move(f)));
        }
        
        explicit coroutine(std::deque<Part> parts) : parts_(std::move(parts)) {
        }
        
        ~coroutine() = default;
        
        coroutine(const coroutine &) = delete;
        
        coroutine(coroutine &&) = default;
        
        coroutine &operator=(const coroutine &) = delete;
        
        coroutine &operator=(coroutine &&) = default;
        
        R operator()(Args ... args) {
            auto pop_finished = scope_exit([this]() {
                if (this->parts_.front().is_finished()) this->parts_.pop_front();
            });
            return parts_.front().call(std::forward<Args>(args) ...);
        }
        
        bool is_finished() const {
            return parts_.empty();
        }
        
    private:
        std::deque<Part> parts_;
    };
}

#endif
