#include "threadingwrapper.h"

void PrimT::ThreadSleep_milli(unsigned int ms)
{
    boost::this_thread::sleep_for(boost::chrono::milliseconds(ms));
}

void PrimT::ThreadSleep_micro(unsigned int us)
{
    boost::this_thread::sleep_for(boost::chrono::microseconds(us));
}
