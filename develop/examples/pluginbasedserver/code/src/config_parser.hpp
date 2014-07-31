#ifndef _HEADER_FILE_CONFIG_PARSER_HPP_
#define _HEADER_FILE_CONFIG_PARSER_HPP_

class ConfigParser
{
public:
    typedef unordered_map < std::string , std::string > T_MAPCONFIG;
    
    ConfigParser()
    {
    }
    ~ConfigParser()
    {
    }
private:
    T_MAPCONFIG _config;
};

#endif // _HEADER_FILE_CONFIG_PARSER_HPP_
