
#include "common.hpp"
#include "plugin_interface.hpp"

class PlugA : public IPlug
{
public:
    PlugA()
    {
        fprintf(stdout,"PlugA construct. \n");
    }
    ~PlugA()
    {
        fprintf(stdout,"PlugA destruct. \n");
    }
    
    int Init()
    {
        fprintf(stdout,"PlugA Init. \n");
        return 0;
    }
    int Run()
    {
        fprintf(stdout,"PlugA Run. \n");
        return 0;
    }
    int Final()
    {
        fprintf(stdout,"PlugA Final. \n");
        return 0;
    }
};

#ifdef __cplusplus
extern "C" {
#endif

IPlug * p=NULL;

// Not thread-safe!
IPlug * GetInst()
{
    if(NULL==p)
    {
        try{
            p =new PlugA();
        }catch(...){
            p=NULL;
        }
    }
    return p;
}

void ReleaseInst()
{
    delete p;
    p =NULL;
}

#ifdef __cplusplus
}
#endif
