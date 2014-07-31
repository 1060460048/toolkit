
#include "plugin_mgr.hpp"
#include "plugin_interface.hpp"

PluginMgr::PluginMgr(const char * plugPath):
    _plugPath(plugPath)
{
    if(0!=IsFileExist(_plugPath.c_str()))
        throw "error";
    if(_plugPath.size()>0 && _plugPath[_plugPath.size()-1]!='/')
        _plugPath +="/";
}

PluginMgr::~PluginMgr()
{
}

std::string PluginMgr::GetPluginName(const std::string & rbuf)
{
    std::string pluginName("plug_");
    // Here find the name from configure.
    
    pluginName +=rbuf;
    return pluginName;
}
std::string PluginMgr::GetDllName(const std::string & pluginName)
{
    std::string dllName(512,'\0');
    snprintf(&dllName[0],dllName.size(),"%slib%s.so",_plugPath.c_str(),pluginName.c_str());
    return dllName;
}

void PluginMgr::Run(const std::string & rbuf)
{
    do{
        std::string pluginName =GetPluginName(rbuf);
        shared_ptr<Dll> p;
        
        T_PLUGINNAME::iterator it =_pluginName.find(pluginName);    
        if (it==_pluginName.end())
        {
            try{
                std::string dllName =GetDllName(pluginName);
                if(0!=IsFileExist(dllName.c_str()))
                    throw "error";
                p.reset( new Dll(dllName.c_str()) );
                _pluginName.insert( std::make_pair(pluginName,p) );
            }catch (...){
                break;
            }
        }
        else
        {
            p =it->second;
        }

        CallFunction(p);
    }while(0);
}

void PluginMgr::CallFunction(shared_ptr<Dll> p)
{
    do{
        std::string funcGetInst ="GetInst";        
        FUNC_GetInst pFuncGetInst =NULL;
        *(void**)(&pFuncGetInst) =p->GetFunction(funcGetInst.c_str());
//        pFuncGetInst =(FUNC_GetInst)p->GetFunction(funcGetInst.c_str());
        if(NULL==pFuncGetInst)
        {
            fprintf(stdout,"NULL==pFuncGetInst \n" );
            break;
        }
        
        std::string funcReleaseInst ="ReleaseInst";
        FUNC_ReleaseInst pFuncGetReleaseInst =NULL;                
        *(void**)(&pFuncGetReleaseInst) =p->GetFunction(funcReleaseInst.c_str());
//        pFuncGetReleaseInst =(FUNC_ReleaseInst)p->GetFunction(funcReleaseInst.c_str());        
        if(NULL==pFuncGetReleaseInst)
        {
            fprintf(stdout,"NULL==pFuncGetReleaseInst \n" );
            break;
        }
        
        IPlug * pInst = (*pFuncGetInst)();
        if(NULL!=pInst)
        {
            pInst->Init();
            pInst->Run();
            pInst->Final();
        }
        else
            fprintf(stdout,"NULL==pInst \n" );
        
        (*pFuncGetReleaseInst)();
    }while(0);
}
