#include "threadingwrapper.h"

#ifdef __unix
int result = XInitThreads();
#endif

void ThreadSleep(unsigned int ms)
{
    boost::this_thread::sleep_for(boost::chrono::milliseconds(ms));
}
