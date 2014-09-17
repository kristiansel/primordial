// Thread initialization needs to be called on unix

#ifndef X_INIT_THREADS_H
#define X_INIT_THREADS_H

#ifdef __unix

#include <X11/Xlib.h>

extern int result;

#endif // UNIX

#endif // X_INIT_THREADS_H
