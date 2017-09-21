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
        
    private:
        T f_;
    };
    
    template <typename T>
    auto scope_exit(T &&f) {
        return ScopeExit<T>(std::forward<T>(f));
    }
}

#endif
