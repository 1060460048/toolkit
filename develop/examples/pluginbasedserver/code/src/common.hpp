#ifndef _HEADER_FILE_COMMON_HPP_
#define _HEADER_FILE_COMMON_HPP_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <dlfcn.h>

#include <string>
#include <vector>
#include <tr1/memory>
#include <tr1/unordered_map>


inline int IsFileExist(const char * file)
{
    int rt =access(file, R_OK);
    if(0!=rt)
    {
        fprintf(stdout,"Failed access,%s,%d,%s\n",file,errno,strerror(errno) );
    }
    return rt;
}

#endif // _HEADER_FILE_COMMON_HPP_
