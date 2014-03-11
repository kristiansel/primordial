#ifndef THREADWRAPPER_H
#define THREADWRAPPER_H

#ifdef WINDOWS

#include <boost/thread.hpp>

using Thread = boost::thread;
using Mutex = boost::mutex;
using LockGuard = boost::lock_guard<boost::mutex>;

#else // NOT WINDOWS

#include <thread>

using Thread = std::thread;
using Mutex = std::mutex;
using LockGuard = std::lock_guard<std::mutex>;

#endif // WINDOWS

#endif // THREADWRAPPER_H

