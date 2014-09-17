#ifndef THREADWRAPPER_H
#define THREADWRAPPER_H

//#ifdef WINDOWS
//
//#include <boost/thread.hpp>
//
//using Thread = boost::thread;
//using Mutex = boost::mutex;
//using LockGuard = boost::lock_guard<boost::mutex>;
//
//#else // NOT WINDOWS

//#include <thread>
//#include <mutex>
////
//using Thread = std::thread;
//using Mutex = std::mutex;
//using LockGuard = std::lock_guard<std::mutex>;


#include <boost/thread.hpp>
#include <boost/chrono.hpp>

namespace PrimT
{
    using Thread = boost::thread;
    using Mutex = boost::mutex;
    using LockGuard = boost::lock_guard<boost::mutex>;

    extern void ThreadSleep_milli(unsigned int ms);
    extern void ThreadSleep_micro(unsigned int us);
};

//#endif // NOT WINDOWS

#endif // THREADWRAPPER_H

