#ifndef _HEADER_FILE_PLUGIN_INTERFACE_HPP_
#define _HEADER_FILE_PLUGIN_INTERFACE_HPP_

class IPlug
{
public:
    IPlug()
    {
    }
    virtual ~IPlug()
    {
    }
    virtual int Init() =0;
    virtual int Run() =0;
    virtual int Final() =0;
};


typedef IPlug * (*FUNC_GetInst)();
typedef void (*FUNC_ReleaseInst)();


#endif // _HEADER_FILE_PLUGIN_INTERFACE_HPP_
