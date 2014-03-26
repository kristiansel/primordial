#ifndef GLOBAL_H
#define GLOBAL_H

#include "resourcemanager.h"
#include "mesh.h"
#include "texture.h"
#include "skeleton.h"
#include "sound.h"
#include <functional>
#include <vector>
//

typedef void(Texture::*TexMethod)();
typedef std::vector<Texture*> TexQueue;

typedef void(Mesh::*MeshMethod)();
typedef std::vector<Mesh*> MeshQueue;

// the good old dirty global variables
namespace global
{
    extern ResourceManager<Mesh>     mesh_manager;
    extern ResourceManager<Texture>   tex_manager;
    extern ResourceManager<Skeleton> skel_manager;
    extern ResourceManager<Sound>   sound_manager;

//    extern std::function<TexMethod> texDelete;
//    extern std::function<TexMethod> texCreate;
//    extern TexQueue tex_to_create;
//    extern TexQueue tex_to_delete;
//    extern void execTexTasks();
//
//    extern std::function<MeshMethod> meshDelete;
//    extern std::function<MeshMethod> meshCreate;
//    extern MeshQueue mesh_to_create;
//    extern MeshQueue mesh_to_delete;
//    extern void execMeshTasks();
}

#endif // GLOBAL_H
