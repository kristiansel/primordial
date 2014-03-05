#ifndef THREADWRAPPER_H
#define THREADWRAPPER_H

#ifdef WINDOWS

#include <boost/thread.hpp>

using Thread = boost::thread;

#else // NOT WINDOWS

using Thread = std::thread;

#endif // WINDOWS

#endif // THREADWRAPPER_H

