
#include "common.hpp"
#include "plugin_interface.hpp"

class PlugC : public IPlug
{
public:
    PlugC()
    {
        fprintf(stdout,"PlugC construct. \n");
    }
    ~PlugC()
    {
        fprintf(stdout,"PlugC destruct. \n");
    }
    
    int Init()
    {
        fprintf(stdout,"PlugC Init. \n");
        return 0;
    }
    int Run()
    {
        fprintf(stdout,"PlugC Run. \n");
        return 0;
    }
    int Final()
    {
        fprintf(stdout,"PlugC Final. \n");
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
            p =new PlugC();
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
