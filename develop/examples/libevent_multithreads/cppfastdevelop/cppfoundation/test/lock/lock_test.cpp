
#include "cppfoundation/cf_lock.hpp"

int main(int argc,char * argv[])
{
    cf::RawFileRWMutex filelock("./lock_test.lock");
    bool lockOk =filelock.TryWriteLock();
    if(lockOk)
    {
        printf("Lock file ok! Wait 10 seconds. \n");
        usleep(10*1000000);
    }
    else
    {
        printf("Lock file failed! exit. \n");
    }
    return 0;
}
