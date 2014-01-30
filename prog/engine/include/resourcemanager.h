#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "mesh.h"
#include "texture.h"

#include <unordered_map>
#include <memory>
#include <string>


using namespace std; /// STL

class ResourceManager
{
public:
    template <class Type> struct ResCounter
    {
        ResCounter() : counter(0) {};
        ResCounter(shared_ptr<Type> res_ptr_in) : counter(0) { res_ptr = res_ptr_in; };
        shared_ptr<Type> res_ptr;
        int counter;
    };

public:
    ResourceManager();
    virtual ~ResourceManager();

    weak_ptr<Mesh>          getMeshptrFromKey(  string mesh_key_in  );
    weak_ptr<Texture>       getTexptrFromKey(   string tex_key_in   );

protected:
private:
    const string basefolder;
    const string modelfolder;
    const string texturefolder;
    unordered_map<string, ResCounter<Mesh>>             meshes;
//    unordered_map<string, shared_ptr<Mesh>>             meshes;
    unordered_map<string, ResCounter<Texture>>      textures;
};

#endif // RESOURCEMANAGER_H
