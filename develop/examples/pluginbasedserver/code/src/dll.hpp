#ifndef _HEADER_FILE_DLL_HPP_
#define _HEADER_FILE_DLL_HPP_

#include "common.hpp"

class Dll
{
public:
    Dll(const char *filename)
    {
        fprintf(stdout,"DLL open,%s. \n",filename);
        _handle =dlopen(filename, RTLD_LAZY);
        if (!_handle) {
            fprintf(stderr, "Failed dlopen, %s\n", dlerror());
            throw "exception";
        }
        dlerror();
    }   
    ~Dll()
    {
        fprintf(stdout,"DLL close . \n");
        int rt =dlclose(_handle);
        if (0!=rt) {
            fprintf(stderr, "Failed dlclose, %s\n", dlerror());
        }
        
    }
    void * GetFunction(const char *funcName)
    {
        void * p =dlsym(_handle, funcName);
        if (NULL==p) {
            fprintf(stderr, "Failed dlsym, %s\n", dlerror());
        }
        return p;
    }
private:
    void * _handle;
};

#endif // _HEADER_FILE_DLL_HPP_
