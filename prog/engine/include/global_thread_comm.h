#ifndef GLOBAL_THREAD_COMM_H
#define GLOBAL_THREAD_COMM_H

#include <queue>
#include "threadingwrapper.h"

namespace g_thread_com
{
    extern std::queue<std::pair<int, int>> mesh_to_del;
    extern PrimT::Mutex mesh_to_del_mx;
}

#endif // GLOBAL_THREAD_COMM_H
