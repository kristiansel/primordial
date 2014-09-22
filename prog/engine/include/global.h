#ifndef GLOBAL_H
#define GLOBAL_H

#include "resourcemanager.h"
#include "mesh.h"
#include "texture.h"
#include "skeleton.h"
#include "sound.h"
#include <functional>
#include <vector>

// the good old dirty global variables
namespace global
{
    extern ResourceManager<Mesh>     mesh_manager;
    extern ResourceManager<Texture>   tex_manager;
    extern ResourceManager<Skeleton> skel_manager;
    extern ResourceManager<Sound>   sound_manager;
}

#endif // GLOBAL_H
