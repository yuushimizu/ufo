#ifndef ufo_functional_partial
#define ufo_functional_partial

namespace ufo {
    template <typename F, typename ... FixedArgs>
    class partial {
    public:
        constexpr partial(F f, FixedArgs ... fixed_args) : f_(std::move(f)), fixed_args_ {std::move(fixed_args) ...} {
        }
        
        template <typename ... Args>
        constexpr decltype(auto) operator()(Args && ... args) const & {
            return invoke(f_, fixed_args_, indices, std::forward<Args>(args) ...);
        }

        template <typename ... Args>
        constexpr decltype(auto) operator()(Args && ... args) & {
            return invoke(f_, fixed_args_, indices, std::forward<Args>(args) ...);
        }
        
        template <typename ... Args>
        constexpr decltype(auto) operator()(Args && ... args) && {
            return invoke(std::move(f_), std::move(fixed_args_), indices, std::forward<Args>(args) ...);
        }
        
    private:
        F f_;
        std::tuple<FixedArgs ...> fixed_args_;
        
        static constexpr const auto indices = std::make_index_sequence<sizeof...(FixedArgs)> {};
        
        template <typename Fn, typename Front, std::size_t ... I, typename ... Back>
        constexpr static decltype(auto) invoke(Fn &&f, Front &&front, std::index_sequence<I ...>, Back && ... back) {
            return std::invoke(std::forward<Fn>(f), std::get<I>(std::forward<Front>(front)) ..., std::forward<Back>(back) ...);
        }
    };
}

#endif
