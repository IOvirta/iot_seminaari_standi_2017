#ifndef IOVIRTA_IOT_CONF_CONF_HPP
#define IOVIRTA_IOT_CONF_CONF_HPP

#include <string>
#include <utility>
#include <vector>
#include <type_traits>
#include <sstream>
//#include <iostream>
#include "tree.hpp"

namespace iovirta_iot
{
namespace conf
{
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
        if (!read_configuration_file())
        {} // throw
    }
    ~Configuration() {}
    // TODO: rule of 5

    //template <typename T, typename... Path, typename = std::enable_if<std::is_arithmetic<T>::value>>
    template <typename T, typename... Path>
    typename std::enable_if<std::is_arithmetic<T>::value, T>::type
    get(Path... path)
    {
        return detail::string_to_int<T>(config_data_.get(path...));
    }

    template <typename T, typename... Path>
    typename std::enable_if<std::is_same<T, std::string>::value, T>::type get(Path... path)
    {
        return config_data_.get(path...);
    }

}; // class Configuration
} // namespace conf
} // namespace iovirta_iot

#endif // IOVIRTA_IOT_CONF_CONF_HPP
