/* TODO Configuration::set()
        save()
*/

#ifndef IOVIRTA_IOT_CONF_CONF_HPP
#define IOVIRTA_IOT_CONF_CONF_HPP

#include <string>
#include <vector>
#include <type_traits>
#include <sstream>
#include <functional>

#include "tree.hpp"
#include <iostream>
namespace iovirta_iot
{
namespace conf
{
template <typename ValueType = std::string>
struct Item
{
    std::string name;
    ValueType value;

    typedef ValueType value_type;
}; // struct Item

namespace detail
{
template <typename T, typename = std::enable_if<std::is_arithmetic<T>::value>>
T string_to_int(const std::string &string)
{
    std::stringstream ss;
    ss << string;
    T ret;
    ss >> ret;
    return ret;
}

template <typename T>
struct is_vector : std::false_type {};
template <typename T>
struct is_vector<std::vector<T>> : std::true_type {};

template <typename T>
struct is_item : std::false_type {};
template <typename T>
struct is_item<Item<T>> : std::true_type {};
} // namespace detail

class Configuration
{
private:
    std::string conf_file_;

    tree::Tree<> config_data_;

    bool read_configuration_file();

public:
    Configuration(const std::string &conf_file) : Configuration(conf_file, std::vector<std::string>()) {}
    Configuration(const std::string &conf_file, const std::vector<std::string> &args) : conf_file_(conf_file)
    {
        // TODO args
        if (!read_configuration_file())
        {} // throw
    }
    ~Configuration() {}
    // TODO: rule of 5

    // get<Item<std::string>>(..)
    template <typename T, typename... Path>
    typename std::enable_if<detail::is_item<T>::value && std::is_arithmetic<typename T::value_type>::value, T>::type get(Path... path)
    {
        auto i = config_data_.get(path...);

        return {i->name, detail::string_to_int<typename T::value_type>(i->value)};
    }

    // get<Item<arithmetic type>(..)
    template <typename T, typename... Path>
    typename std::enable_if<std::is_same<T, Item<std::string>>::value, T>::type get(Path... path)
    {
        auto i = config_data_.get(path...);

        return {i->name, i->value};
    }

    // get<arithmetic type>(..)
    template <typename T, typename... Path>
    typename std::enable_if<std::is_arithmetic<T>::value, T>::type get(Path... path)
    {
        return detail::string_to_int<T>(config_data_.get(path...)->value);
    }

    // get<std::string>(..)
    template <typename T, typename... Path>
    typename std::enable_if<std::is_same<T, std::string>::value, T>::type get(Path... path)
    {
        return config_data_.get(path...)->value;
    }

    // get<std::vector<arithmetic type>>(..)
    template <typename T, typename... Path>
    typename std::enable_if<detail::is_vector<T>::value && std::is_arithmetic<typename T::value_type>::value, T>::type get(Path... path)
    {
        std::vector<std::string> values_str = get<std::vector<std::string>>(path...);

        T values;
        for (auto &v : values_str)
            values.push_back(detail::string_to_int<typename T::value_type>(v));

        return values;
    }

    // get<std::vector<std::string>>(..)
    template <typename T, typename... Path>
    typename std::enable_if<std::is_same<T, std::vector<std::string>>::value, T>::type get(Path... path)
    {
        T values;
        tree::Node<> *root = config_data_.get(path...);

        std::function<void(tree::Node<>*)> get_values = [&get_values, &values] (tree::Node<> *node)
        {
            for (auto it = node->childs.begin(); it != node->childs.end(); ++it)
            {
                values.push_back((*it)->value);

                if ((*it)->childs.size() != 0)
                    get_values(*it);
            }
        };

        get_values(root);


        return values;
    }

    // get<std::vector<Item<K, V>>>(..)
    // TODO

}; // class Configuration
} // namespace conf
} // namespace iovirta_iot

#endif // IOVIRTA_IOT_CONF_CONF_HPP
