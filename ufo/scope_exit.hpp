#ifndef ufo_scope_exit
#define ufo_scope_exit

namespace ufo {
    template <typename T>
    class ScopeExit {
    public:
        ScopeExit(T f) : f_(std::move(f)) {
        }
        
        ~ScopeExit() {
            f_();
        }
        
        ScopeExit(const ScopeExit &) = delete;
        
        ScopeExit(ScopeExit &&) = delete;
        
        ScopeExit &operator=(const ScopeExit &) = delete;
        
        ScopeExit &operator=(ScopeExit &&) = delete;
        
    private:
        T f_;
    };
    
    template <typename T>
    auto scope_exit(T &&f) {
        return ScopeExit<T>(std::forward<T>(f));
    }
}

#endif
