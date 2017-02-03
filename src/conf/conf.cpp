#include "conf/conf.hpp"
#include "conf/parser/default.hpp"
#include "conf/parser/ini.hpp"

#include <fstream>


bool iovirta_iot::conf::Configuration::read_configuration_file()
{
    std::string conf_file_extension = conf_file_.substr(conf_file_.find_last_of(".") + 1);

    parser::ConfFileParser *parser;

    if (conf_file_extension == "ini")
        parser = new parser::ConfFileParserINI(conf_file_);
    else
        parser = new parser::ConfFileParserDefault(conf_file_);

    parser->parse_configuration_file(config_data_);

    delete parser;

    return true; // TODO
}
