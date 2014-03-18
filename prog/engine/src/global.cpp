#include "global.h"

ResourceManager<Mesh>     global::mesh_manager;
ResourceManager<Texture>   global::tex_manager;
ResourceManager<Skeleton> global::skel_manager;

//////////////////////////////////////////////////////////////////////////

//
////std::function<TexMethod> global::texDelete = &Texture::deleteGL;
////std::function<TexMethod> global::texCreate = &Texture::createGL;
//
//TexQueue global::tex_to_create;
//TexQueue global::tex_to_delete;
//
//void global::execTexTasks();
//
////////////////////////////////////////////////////////////////////////////
//
////std::function<MeshMethod> global::meshDelete = &Mesh::deleteGL();
////std::function<MeshMethod> global::meshCreate = &Mesh::createGL();
//
//MeshQueue global::mesh_to_create;
//MeshQueue global::mesh_to_delete;
//
//void global::execMeshTasks();
