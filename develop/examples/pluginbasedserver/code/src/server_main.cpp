
#include "common.hpp"
#include "plugin_mgr.hpp"


int main(int argc,char * argv[])
{
    if(argc<2)
    {
        fprintf(stdout,"Usage: %s <plugin path> \n",argv[0]);
        return 1;
    }
    fprintf(stdout,"Please input plugin name(e.g. 'a'), type q to exit ...\n");
    
    std::string plugPath(argv[1]);
    PluginMgr pm(plugPath.c_str());
    
    std::string rbuf(1024,'\0');
    while(1)
    {
        bzero(&rbuf[0],rbuf.size());
        read(0,&rbuf[0],rbuf.size());
        rbuf[strlen(rbuf.c_str())-1] ='\0';
        if(rbuf[0]=='Q' || rbuf[0]=='q')
        {
            break;
        }
        pm.Run(rbuf);
        fprintf(stdout,"\n");
    }    
    return 0;
}
