#include "global_thread_comm.h"

std::queue<std::pair<int, int>> g_thread_com::mesh_to_del;
PrimT::Mutex g_thread_com::mesh_to_del_mx;
