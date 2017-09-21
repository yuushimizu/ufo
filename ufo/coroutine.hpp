#ifndef ufo_coroutine
#define ufo_coroutine

#include <functional>
#include <initializer_list>
#include <deque>
#include <experimental/optional>
#include "scope_exit.hpp"

namespace ufo {
    template <typename>
    class coroutine;
    
    template <typename R, typename ... Args>
    class coroutine<R(Args ...)> {
    public:
        class Part {
        private:
            class Impl {
            public:
                virtual ~Impl() = default;
                
                virtual R operator()(Args ...) = 0;
                
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
            
            class Normal : public Impl {
            public:
                Normal(std::function<R(Args ...)> f) : f_(std::move(f)) {
                }
                
                virtual R operator()(Args ... args) {
                    auto finish = scope_exit([this]() {
                        this->finish();
                    });
                    return f_(std::forward<Args>(args) ...);
                }
                
            private:
                std::function<R(Args ...)> f_;
            };
            
            class Nested : public Impl {
            public:
                Nested(std::function<coroutine<R(Args ...)>(Args ...)> f) : f_(std::move(f)) {
                }
                
                virtual R operator()(Args ... args) {
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
                std::function<coroutine<R(Args ...)>(Args ...)> f_;
                std::experimental::optional<coroutine<R(Args ...)>> coro_ = std::experimental::nullopt;
                bool called_ = false;
            };
            
            class Delegate : public Impl {
            public:
                Delegate(coroutine<R(Args ...)> coro) : coro_(std::move(coro)) {
                }
                
                virtual R operator()(Args ... args) {
                    auto finish = scope_exit([this]() {
                        if (this->coro_.is_finished()) this->finish();
                    });
                    return coro_(std::forward<Args>(args) ...);
                }
                
            private:
                coroutine<R(Args ...)> coro_;
            };
        public:
            explicit Part(std::function<R(Args ...)> f) : impl_(std::make_unique<Normal>(std::move(f))) {
            }
            
            explicit Part(std::function<coroutine<R(Args ...)>(Args ...)> f) : impl_(std::make_unique<Nested>(std::move(f))) {
            }
            
            explicit Part(coroutine<R(Args ...)> coro) : impl_(std::make_unique<Delegate>(std::move(coro))) {
            }
            
            R operator()(Args ... args) {
                return (*impl_)(std::forward<Args>(args) ...);
            }
            
            bool is_finished() const {
                return impl_->is_finished();
            }
            
        private:
            std::unique_ptr<Impl> impl_;
        };
        
        coroutine() : parts_ {} {
        }

        template <typename F, typename ... Fs>
        explicit coroutine(F f, Fs ... fs) : coroutine(std::move(fs) ...) {
            parts_.push_front(Part(std::move(f)));
        }
        
        explicit coroutine(std::deque<Part> parts) : parts_(std::move(parts)) {
        }
        
        coroutine(const coroutine &) = delete;
        
        coroutine(coroutine &&) = default;
        
        coroutine &operator=(const coroutine &) = delete;
        
        coroutine &operator=(coroutine &&) = default;
        
        R operator()(Args ... args) {
            auto pop_finished = scope_exit([this]() {
                if (this->parts_.front().is_finished()) this->parts_.pop_front();
            });
            return parts_.front()(std::forward<Args>(args) ...);
        }
        
        bool is_finished() const {
            return parts_.empty();
        }
        
    private:
        std::deque<Part> parts_;
    };
}

#endif
