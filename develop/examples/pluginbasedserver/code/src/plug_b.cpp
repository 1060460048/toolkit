
#include "common.hpp"
#include "plugin_interface.hpp"

class PlugB : public IPlug
{
public:
    PlugB()
    {
        fprintf(stdout,"PlugB construct. \n");
    }
    ~PlugB()
    {
        fprintf(stdout,"PlugB destruct. \n");
    }
    
    int Init()
    {
        fprintf(stdout,"PlugB Init. \n");
        return 0;
    }
    int Run()
    {
        fprintf(stdout,"PlugB Run. \n");
        return 0;
    }
    int Final()
    {
        fprintf(stdout,"PlugB Final. \n");
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
            p =new PlugB();
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
