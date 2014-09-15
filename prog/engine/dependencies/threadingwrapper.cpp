#include "threadingwrapper.h"

#ifdef __unix
int result = XInitThreads();
#endif

void ThreadSleep_milli(unsigned int ms)
{
    boost::this_thread::sleep_for(boost::chrono::milliseconds(ms));
}

void ThreadSleep_micro(unsigned int us)
{
    boost::this_thread::sleep_for(boost::chrono::microseconds(us));
}
