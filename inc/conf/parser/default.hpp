#ifndef IOVIRTA_IOT_CONF_PARSER_DEFAULT_HPP
#define IOVIRTA_IOT_CONF_PARSER_DEFAULT_HPP

#include <string>
#include <vector>

#include "conf/parser/conf_file_parser.hpp"
#include "tree.hpp"

namespace iovirta_iot
{
namespace conf
{
namespace parser
{
class ConfFileParserDefault : public ConfFileParser
{
public:
    ConfFileParserDefault(std::string f) : ConfFileParser(f) {}
    ~ConfFileParserDefault() {}
    void parse_configuration_file(tree::Tree<> &tree)
    {
        auto lines = read_lines();

        /* #kommentti
           avain arvo */

        for (auto &line : lines)
        {
            std::string data = line.substr(0, line.find_first_of("#"));

            if (data.size() == 0)
                continue;

            std::string key = data.substr(0, data.find_first_of(" "));
            std::string value = data.substr(data.find_first_of(" "));

            tree.add_node(key, value);
        }
    }
}; // class ConfFileParserDefault
} // namespace parser
} // namespace conf
} // namespace iovirta_iot

#endif // IOVIRTA_IOT_CONF_PARSER_DEFAULT_HPP
