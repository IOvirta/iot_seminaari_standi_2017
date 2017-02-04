#ifndef IOVIRTA_IOT_CONF_PARSER_INI_HPP
#define IOVIRTA_IOT_CONF_PARSER_INI_HPP

#include "conf/parser/conf_file_parser.hpp"
#include "tree.hpp"

namespace iovirta_iot
{
namespace conf
{
namespace parser
{
class ConfFileParserINI : public ConfFileParser
{
public:
    ConfFileParserINI(std::string f) : ConfFileParser(f) {}
    ~ConfFileParserINI() {}

    void parse_configuration_file(tree::Tree<> &tree)
    {
        
        // TODO: stripata stringit
        auto lines = read_lines();
        tree::Node<> *section = nullptr;
        /* [section]
            key = value ;kommenti*/
        for (auto &line : lines)
        {
            auto data = line.substr(0, line.find_first_of(";"));

            if (data.size() == 0)
                continue;

            auto section_begin = data.find_first_of("[");
            auto section_end = data.find_first_of("]");

            if (section_begin == std::string::npos || section_end == std::string::npos)
            {
                auto key = data.substr(0, data.find_first_of("="));
                auto value = data.substr(data.find_first_of("=") + 1);

                if (section != nullptr)
                    tree.add_node(section, key, value);
                else
                    tree.add_node(key, value);
            }
            else
            {
                section = tree.add_node(data.substr(data.find_first_of("[") + 1, data.find_first_of("]")-data.find_first_of("[")-1));
            }
        }
    }
}; // class ConfFileParserINI
} // namespace parser
} // namespace conf
} // namespace iovirta_iot

#endif // IOVIRTA_IOT_CONF_PARSER_INI_HPP
