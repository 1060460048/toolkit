
#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/helpers/loglog.h>
#include <log4cplus/helpers/stringhelper.h>
#include <log4cplus/loggingmacros.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;

void
printDebug()
{
	for(int i=0;i<1;i++)
    {
        std::string buf(8192,'a');
		LOG4CPLUS_INFO(Logger::getRoot(), buf.c_str() );
    }
}

int
main1()
{
    log4cplus::initialize ();
    try {
        PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("./log.properties"));
        pid_t pid =fork();
        if(pid<0)
        {
            printf("Failed fork \n");
            exit(0);
        }
        else if(pid==0)
        {
            printDebug();
            return 0;
        }
        
        printDebug();
    }
    catch(...) {
        tcout << "Exception..." << endl;
        LOG4CPLUS_FATAL(Logger::getRoot(), "Exception occured...");
    }
    return 0;
}


int
main()
{
    PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("./log.properties"));
    printDebug();
    return 0;
}

