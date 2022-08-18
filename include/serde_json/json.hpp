#ifndef KIE_TOOLBOX_SERDE_JSON_JSON_HPP
#define KIE_TOOLBOX_SERDE_JSON_JSON_HPP

#include <array>
#include <vector>
#include <list>
#include <type_traits>
#include <string_view>
#include <string>


#include <boost/pfr.hpp>
#include <nlohmann/json.hpp>
#include <serde/field.hpp>


/** @brief the main namespace of this library
 *
 *
 */
namespace kie::serde_json
{

    /** @brief This namespace contains some trait used by this library at compile time.
     *
     * It help robust this library with some compile time check with concept.
     * At the same time, it make some overload decision posible or simpler thanks to concept.
     *
     */
    namespace type_trait
    {

        /** @brief A helper variable indicating that this is not implemented.
         *
         * Just a helper variable and not used for now.
         *
         */
        template <class...>
        constexpr std::false_type not_implemented{};

        /** @brief Check if a class is a specialization of a template.
         *
         * It only applies to the template type with only type parameters. If
         * there are non-type parameters, it fails to check.
         *
         */
        template <typename T, template <typename...> class Z>
        struct is_specialization_of : std::false_type
        {
        };

        /** @brief Check if a class is a specialization of a template.
         *
         * It only applies to the template type with only type parameters. If
         * there are non-type parameters, it fails to check.
         *
         */
        template <typename... Args, template <typename...> class Z>
        struct is_specialization_of<Z<Args...>, Z> : std::true_type
        {
        };

        /** @brief Check if type T is `std::array`
         *
         * With the limitation of `is_specialization_of`, this class
         * helps to check `std::array` because of the second parameter
         * is non-type.
         *
         */
        template <typename T>
        struct is_array_class : std::false_type
        {
        };

        /** @brief Check if type T is `std::array`
         *
         * With the limitation of `is_specialization_of`, this class
         * helps to check `std::array` because of the second parameter
         * is non-type.
         *
         */
        template <typename T, std::size_t size>
        struct is_array_class<std::array<T, size>> : std::true_type
        {
        };

        /** @brief A concept checks if type T is a `container`.
         *
         * Container means three things here, a vector, a list and an array.
         *
         */
        template <typename T>
        concept is_container = is_specialization_of<T, std::vector>::value ||
            is_specialization_of<T, std::list>::value ||
            is_array_class<T>::value;

        /** @brief A concept checks if type T is a `dynamic container`
         *
         * Dynamic container means std::vector and std::list here.
         *
         */
        template <typename T>
        concept is_dynamic_container = (is_specialization_of<T, std::vector>::value ||
                                        is_specialization_of<T, std::list>::value) &&
                                       requires
        {
            typename T::value_type;
        };
    }

    /** @brief to_json overload that only accept container.
     *
     * This is just a declaration for overload.
     *
     */
    template <type_trait::is_container T>
    nlohmann::json to_json(const T &t);

    /** @brief The version of to_json that accepts all the types
     *
     * This function will loop over the fields of T and then write them
     * to a json object.
     *
     * Notice that the input should be of aggregate type. Or a compile error
     * will occurs.
     *
     */
    template <typename T>
    nlohmann::json to_json(const T &t)
    {
        nlohmann::json j;
        boost::pfr::for_each_field(t, [&j]<typename TT>(const TT &field, std::size_t)
                                   {
            if constexpr(kie::serde::type_trait::is_field<TT>::value){
                if constexpr(std::is_class_v<typename TT::Type> && !std::is_same_v<typename TT::Type, std::string>){
                    j[std::string{field.tag()}] = to_json(field.value);
                }else{
                    j[std::string{field.tag()}] = field.value;
                }
            } });
        return j;
    }

    /** @brief A specialization of to_json for std::string.
     *
     * String is a special type for this library. It's a class, but should
     * not be treated as a class which will be looped over and std::string can
     * be serialized or deserialized to/from json directly.
     *
     */
    template <>
    nlohmann::json to_json(const std::string &)
    {
        nlohmann::json j;
        return j; // return null for string without wrapping with Field
    }

    /** @brief to_json overload that only accept container.
     *
     * This is the implementation version. It loops the container and
     * store the items if they are of Field type.
     *
     */
    template <type_trait::is_container T>
    nlohmann::json to_json(const T &t)
    {
        nlohmann::json j;
        for (const auto &item : t)
        {
            if constexpr (std::is_class_v<std::decay_t<decltype(item)>> && !std::is_same_v<std::decay_t<decltype(item)>, std::string>)
            {
                j.push_back(to_json(item));
            }
            else
            {
                j.push_back(item);
            }
        }
        return j;
    }

    /** @brief This namespace contains some function used internally.
     *
     * They are all for some overload resolution.
     *
     */
    namespace impl
    {
        /** @brief Convert nlohmann::json directly to T
         *
         * In this function, T should be the type which is supported by nlohmann_json
         *
         * @param j The json object that contains only one thing.
         */
        template <typename T>
        T from_json(const nlohmann::json &j)
        {
            T t{};
            j.get_to(t);
            return t;
        }

        /** @brief Convert json object to container.
         *
         * This is a declaration.
         *
         * @param j The json object that contains only one thing.
         */
        template <type_trait::is_dynamic_container T>
        T from_json(const nlohmann::json &j);

        /** @brief Convert json object to an aggregate type.
         *
         * Almost all the time, user should define a class that only contains Fields.
         * And this is of aggregate type. An json object can be converted to it directly
         * with the help of static reflection provided by pfr.
         *
         * @param j The json object that contains only one thing.
         */
        template <typename T>
        requires std::is_aggregate_v<T>
            T from_json(const nlohmann::json &j)
        {
            T t{};
            boost::pfr::for_each_field(t, [&j]<typename TT>(TT &field, std::size_t)
                                       {
                if constexpr(kie::serde::type_trait::is_field<TT>::value){
                    if constexpr(std::is_class_v<typename TT::Type> && !std::is_same_v<typename TT::Type, std::string>){
                        field = impl::from_json<typename TT::Type>(j.at(std::string{field.tag()}));
                    }else{
                        j.at(std::string{field.tag()}).get_to(field.value);
                    }
                } });
            return t;
        }

        /** @brief Convert json object to container.
         *
         * Some json could be of array type, and this function handles such conversion.
         *
         * @param j The json object that contains only one thing.
         */
        template <type_trait::is_dynamic_container T>
        T from_json(const nlohmann::json &j)
        {
            if (!j.is_array())
            {
                return {};
            }
            T t;
            if constexpr (type_trait::is_specialization_of<T, std::vector>::value)
            {
                t.reserve(j.size());
            }
            for (const auto &item : j)
            {
                t.push_back(impl::from_json<std::decay_t<typename T::value_type>>(item));
            }
            return t;
        }
    }

    /** @brief Converting string_view to aggregate type.
     *
     * This is a friendly deserialization function for json.
     *
     * @param json_str a json string.
     *
     */
    template <typename T>
    requires std::is_aggregate_v<T> && std::is_class_v<T>
        T from_json(std::string_view json_str)
    {
        nlohmann::json j = nlohmann::json::parse(json_str);
        T t{};
        boost::pfr::for_each_field(t, [&j]<typename TT>(TT &field, std::size_t)
                                   {
            if constexpr(kie::serde::type_trait::is_field<TT>::value){
                if constexpr(std::is_class_v<typename TT::Type> && !std::is_same_v<typename TT::Type, std::string>){
                    field = impl::from_json<typename TT::Type>(j.at(std::string{field.tag()}));
                }else{
                    j.at(std::string{field.tag()}).get_to(field.value);
                }
            } });
        return t;
    }

    /** @brief Converting string_view to container type.
     *
     * This is a friendly deserialization function for json.
     *
     * @param json_str a json string.
     *
     */
    template <type_trait::is_dynamic_container T>
    T from_json(std::string_view json_str)
    {
        nlohmann::json j = nlohmann::json::parse(json_str);
        if (!j.is_array())
        {
            return {};
        }
        T t;
        if constexpr (type_trait::is_specialization_of<T, std::vector>::value)
        {
            t.reserve(j.size());
        }
        for (const auto &item : j)
        {
            t.push_back(impl::from_json<std::decay_t<typename T::value_type>>(item));
        }
        return t;
    }

} // namespace kie::serde_json

#endif