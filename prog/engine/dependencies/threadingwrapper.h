#ifndef THREADWRAPPER_H
#define THREADWRAPPER_H

#ifdef WINDOWS

#include <boost/thread.hpp>

using Thread = boost::thread;
using Mutex = boost::mutex;
using LockGuard = boost::lock_guard<boost::mutex>;

#else // NOT WINDOWS

//#include <thread>
//#include <mutex>
//
//using Thread = std::thread;
//using Mutex = std::mutex;
//using LockGuard = std::lock_guard<std::mutex>;


#include <boost/thread.hpp>

using Thread = boost::thread;
using Mutex = boost::mutex;
using LockGuard = boost::lock_guard<boost::mutex>;

#endif // WINDOWS

// Thread initialization needs to be called on unix

#ifdef __unix

#include <X11/Xlib.h>

extern int result;

#endif // UNIX

#endif // THREADWRAPPER_H

