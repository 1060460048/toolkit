#ifndef _HEADER_FILE_PLUGIN_MGR_HPP_
#define _HEADER_FILE_PLUGIN_MGR_HPP_

#include "dll.hpp"
#include "common.hpp"

using namespace std::tr1;

class PluginMgr
{
public:
    typedef unordered_map < std::string , shared_ptr<Dll> > T_PLUGINNAME;
    
    PluginMgr(const char * plugPath="./");
    ~PluginMgr();
    void Run(const std::string & rbuf);

private:    
    void CallFunction(shared_ptr<Dll> p);
    std::string GetPluginName(const std::string & rbuf);
    std::string GetDllName(const std::string & pluginName);
    
    T_PLUGINNAME _pluginName;
    std::string _plugPath;
};


#endif // _HEADER_FILE_PLUGIN_MGR_HPP_
