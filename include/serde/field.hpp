#ifndef KIE_TOOLBOX_SERDE_FIELD_HPP
#define KIE_TOOLBOX_SERDE_FIELD_HPP

#include <type_traits>
#include <algorithm>


/** @brief the main namespace of this library
 * 
 * 
 */
namespace kie::serde
{
    /** @brief String literal used as template parameter
     * 
     * For C++20, std::string and std::string_view become constexpr, but they can't be used as template parameter because
     * some reason, maybe string literal doesn't have address in some scenario. So this class helps.
     * 
     * It create a const char array with size N which is a non-type template parameter. This can hold some compile-time string
     * and bind it in some type to do some reflection.
     * 
     * @param N the size of this string. It contains the suffix `\0`.
     */
    template <std::size_t N>
    class StringLiteral
    {
    public:
        static_assert(N >= 1, "string literal size must be more than 1");

        char value[N];

        /** @brief The constructor for `StringLiteral`. It accept a char array as parameter.
         * 
         * 
         * @param str A reference to a const char array, which is used to deduced the template parameter N.
         */
        constexpr StringLiteral(const char (&str)[N])
        {
            std::copy_n(str, N, value);
        }


        /** @brief Convert string literal to `std::string_view`.
         * 
         * This function doesn't change anything in this class so it's marked as const.
         */
        [[nodiscard]] constexpr std::string_view to_string_view() const
        {
            return {value, N - 1};
        }
    };


    /** @brief Field class is used to represent a serializable and deserializable field.
     * 
     * It accepts a type parameter and `StringLiteral` parameter.
     * 
     * Usage:
     * @code
     * Field<int, "field_name"> f1;
     * @endcode
     * 
     * @param T The type of field which is held by Field
     * @param _tag The tag of this field. Used to represent the name of this field.
     */
    template <typename T, StringLiteral _tag>
    struct Field
    {
        using Type = T;
        T value;

        /** @brief The default constructor.
         * 
         */
        Field() = default;

        /** @brief The constructor that accepts other value.
         * 
         * The type of parameter must be the same with T. 
         * This function can both handle lvalue and rvalue.
         * 
         * @param v The value with the same time with T, which is the value of actually field.
         * 
         */
        template<typename U> requires std::is_same_v<T, std::decay_t<U>>
        Field(U&& v)
        {
            value = std::forward<U>(v);
        }


        /** @brief The copy constructor.
         * 
         * Nothing special. Just a copy constructor and copy the value to this.
         * 
         */
        Field(const Field &v)
        {
            value = v.value;
        }

        /** @brief The move constructor
         * 
         * Nothing special. Just a move constructor and move the value to this.
         */
        Field(Field &&v)
        {
            value = std::move(v.value);
        }

        /** @brief The copy assignment operator.
         * 
         * Nothing special. Just a copy assignment operator and copy the value to this.
         * 
         */
        Field &operator=(const Field &v)
        {
            value = v.value;
            return *this;
        }


        /** @brief The move assignment operator.
         * 
         * Nothing special. Just a move assignment operator and move the value to this.
         * 
         */
        Field &operator=(Field &&v)
        {
            value = std::move(v.value);
            return *this;
        }


        /** @brief The assignment operator that accepts other value.
         * 
         * The type of parameter must be the same with T. 
         * This function can both handle lvalue and rvalue.
         * 
         * @param v The value with the same time with T, which is the value of actually field.
         * 
         */
        template<typename U> requires std::is_same_v<T, std::decay_t<U>>
        Field &operator=(U&& v)
        {
            value = std::forward<U>(v);
            return *this;
        }

        /** @brief This function get the tag of this field.
         * 
         * This function does not change anything of this class, so marked as const.
         */
        constexpr std::string_view tag() const
        {
            return _tag.to_string_view();
        }


        /** @brief The implicit conversion function to T&
         * 
         * This function is used to convert Field to any type it wraps.
         * It converts this to a reference to T so that any operation on the 
         * value of T can be applied to this.
         * 
         * For example, if a function accept a reference to T, it can still accept
         * Field as its parameter.
         */
        operator T &()
        {
            return value;
        }
    };

    /** @brief This namespace contains some trait used by this library at compile time.
     * 
     * It help robust this library with some compile time check with concept.
     * At the same time, it make some overload decision posible or simpler thanks to concept.
     * 
     */
    namespace type_trait
    {
        /** @brief Check if type T is a field.
         * 
         */
        template <typename T>
        struct is_field : std::false_type
        {
        };

        /** @brief Check if type T is a field.
         * 
         */
        template <typename T, StringLiteral str>
        struct is_field<Field<T, str>> : std::true_type
        {
        };

    } //namespace type_trait

} // namespace kie::serde


#endif