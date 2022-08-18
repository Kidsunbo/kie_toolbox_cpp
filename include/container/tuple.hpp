#ifndef KIE_TOOLBOX_CONTAINER_TUPLE_HPP
#define KIE_TOOLBOX_CONTAINER_TUPLE_HPP

#include <cstddef>
#include <tuple>
#include <utility>

namespace kie::container
{

    namespace impl
    {
        template <typename... ARGS, typename F, std::size_t... Index>
        constexpr void loop_tuple_impl(std::tuple<ARGS...> &t, F &&f,
                                  std::index_sequence<Index...>)
        {
            (std::forward<F>(f)(std::get<Index>(t)), ...);
        }

        template <typename... ARGS, typename F, std::size_t... Index>
        constexpr void loop_tuple_reverse_impl(std::tuple<ARGS...> &t, F &&f,
                                          std::index_sequence<Index...>)
        {
            constexpr auto size = sizeof...(Index);
            (std::forward<F>(f)(std::get<size - Index - 1>(t)), ...);
        }
    } // namespace impl

    template <typename... ARGS, typename F>
    constexpr void loop_tuple(std::tuple<ARGS...> &t, F &&f)
    {
        impl::loop_tuple_impl(t, std::forward<F>(f), std::make_index_sequence<sizeof...(ARGS)>());
    }

    template <typename... ARGS, typename F>
    constexpr void loop_tuple_reverse(std::tuple<ARGS...> &t, F &&f)
    {
        impl::loop_tuple_reverse_impl(t, std::forward<F>(f), std::make_index_sequence<sizeof...(ARGS)>());
    }


} // namespace kie


#endif
