#ifndef IOVIRTA_IOT_CONF_PARSER_CONF_FILE_PARSER_HPP
#define IOVIRTA_IOT_CONF_PARSER_CONF_FILE_PARSER_HPP

#include <string>
#include <vector>
#include <fstream>
#include <iterator>
#include "tree.hpp"

namespace iovirta_iot
{
namespace conf
{
namespace parser
{
namespace detail
{
class Line
{
    std::string data;
public:
    friend std::istream &operator>>(std::istream &stream, Line &line)
    {
        std::getline(stream, line.data);
        return stream;
    }

    operator std::string() const
    {
        return data;
    }
}; // class Line
} // namespace details

class ConfFileParser
{
private:
    std::string file_;

protected:
    std::vector<std::string> read_lines()
    {
        std::fstream file_in(file_);

        if (!file_in.is_open()) {}
            // throw?

        std::vector<std::string> lines;
        std::copy(std::istream_iterator<detail::Line>(file_in), std::istream_iterator<detail::Line>(),
            std::back_inserter(lines));

        return lines;
    }
public:
    ConfFileParser(std::string file) : file_(file) {}
    virtual ~ConfFileParser() {}
    virtual void parse_configuration_file(tree::Tree<> &) =0;
}; // class ConfFileParser
} // namespace parser
} // namespace conf
} // namespace iovirta_iot

#endif // IOVIRTA_IOT_CONF_PARSER_CONF_FILE_PARSER_HPP
