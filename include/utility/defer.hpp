#ifndef KIE_TOOLBOX_UTILITY_DEFER_HPP
#define KIE_TOOLBOX_UTILITY_DEFER_HPP

#include <functional>

namespace kie::utility
{
    class Defer
    {
        std::function<void()> func = nullptr;

    public:
        Defer &operator=(std::function<void()> func)
        {
            this->func = std::move(func);
            return *this;
        }

        ~Defer()
        {
            if (func != nullptr)
            {
                func();
            }
        }
    };

    class TryDefer
    {
        std::function<void()> func = nullptr;

    public:
        TryDefer &operator=(std::function<void()> func)
        {
            this->func = std::move(func);
            return *this;
        }

        ~TryDefer()
        {
            try
            {
                if (func != nullptr)
                {
                    func();
                }
            }
            catch (std::exception &)
            {
            }
        }
    };
} // namespace kie::utility


#define __defer_cat(x, y) x##y

#define defer_impl(x)                     \
    kie::utility::Defer __defer_cat(__DEFER_NAME, x); \
    __defer_cat(__DEFER_NAME, x) =

#define try_defer_impl(x)                              \
    kie::utility::TryDefer __defer_cat(__DEFER_NAME, x); \
    __defer_cat(__DEFER_NAME, x) =

#define defer defer_impl(__COUNTER__)

#define try_defer try_defer_impl(__COUNTER__)

#define test_counter(x) x;

#define unique_name test_counter(__COUNTER__)

#endif