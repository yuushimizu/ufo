#ifndef ufo_coroutine
#define ufo_coroutine

#include <functional>
#include <initializer_list>
#include <deque>
#include <experimental/optional>
#include "scope_exit.hpp"

namespace ufo {
    template <typename R, typename ... Args>
    class Coroutine {
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
                Nested(std::function<Coroutine<R, Args ...>(Args ...)> f) : f_(std::move(f)) {
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
                std::function<Coroutine<R, Args ...>(Args ...)> f_;
                std::experimental::optional<Coroutine<R, Args ...>> coro_ = std::experimental::nullopt;
                bool called_ = false;
            };
            
            class Delegate : public Impl {
            public:
                Delegate(Coroutine<R, Args ...> coro) : coro_(std::move(coro)) {
                }
                
                virtual R operator()(Args ... args) {
                    auto finish = scope_exit([this]() {
                        if (this->coro_.is_finished()) this->finish();
                    });
                    return coro_(std::forward<Args>(args) ...);
                }
                
            private:
                Coroutine<R, Args ...> coro_;
            };
        public:
            Part(std::function<R(Args ...)> f) : impl_(std::make_unique<Normal>(std::move(f))) {
            }
            
            Part(std::function<Coroutine<R, Args ...>(Args ...)> f) : impl_(std::make_unique<Nested>(std::move(f))) {
            }
            
            Part(Coroutine<R, Args ...> coro) : impl_(std::make_unique<Delegate>(std::move(coro))) {
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
        
        Coroutine() : parts_ {} {
        }
        
        Coroutine(std::deque<Part> parts) : parts_(std::move(parts)) {
        }
        
        Coroutine(const Coroutine &) = delete;
        
        Coroutine(Coroutine &&) = default;
        
        Coroutine &operator=(const Coroutine &) = delete;
        
        Coroutine &operator=(Coroutine &&) = default;
        
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
    
    namespace coroutine_ {
        template <typename R, typename ... Args>
        void create_parts(std::deque<typename Coroutine<R, Args ...>::Part> &parts) {
        }
        
        template <typename R, typename ... Args, typename F, typename ... Fs>
        void create_parts(std::deque<typename Coroutine<R, Args ...>::Part> &parts, F f, Fs ... fs) {
            parts.push_back(typename Coroutine<R, Args ...>::Part(std::move(f)));
            create_parts<R, Args ...>(parts, std::move(fs) ...);
        }
    }
    
    template <typename R, typename ... Args, typename ... Fs>
    auto coroutine(Fs && ... fs) {
        std::deque<typename Coroutine<R, Args ...>::Part> parts {};
        coroutine_::create_parts<R, Args ...>(parts, std::move(fs) ...);
        return Coroutine<R, Args ...>(std::move(parts));
    }
}

#endif
