#include "resourcemanager.h"

ResourceManager::ResourceManager() : basefolder     ("assets_raw")
    ,modelfolder    ("models")
    ,texturefolder  ("textures")

{

}

ResourceManager::~ResourceManager()
{

}

weak_ptr<Mesh> ResourceManager::getMeshptrFromKey(string mesh_key_in)      /// MESH
{
    auto emplace_result = meshes.emplace(mesh_key_in, ResCounter<Mesh>(shared_ptr<Mesh>(new Mesh)));
    /// Type = pair<unordered_map<string, ResCounter<Mesh>>::iterator, bool>

    auto pair_it = emplace_result.first; /// unordered_map<string, ResCounter<Mesh>>::iterator

    bool inserted   = emplace_result.second;
    auto mesh_counter = &(pair_it->second);
    auto mesh_ptr   = mesh_counter->res_ptr;

    if (inserted)
    {
        /// construct the mesh filepath from the key
        string filepath = basefolder + "/" + modelfolder + "/" + mesh_key_in + ".obj";

        mesh_ptr->fromFile(filepath);

        cout << "inserted for " << mesh_key_in << "\n";
    }

    /// Important: Count another reference to the resource
    mesh_counter->counter++;

    cout << "mesh count for " << mesh_key_in << ": " << mesh_counter->counter << "\n";

    return weak_ptr<Mesh>(mesh_ptr);
}

weak_ptr<Texture> ResourceManager::getTexptrFromKey(string tex_key_in)      /// MESH
{
    auto emplace_result = textures.emplace(tex_key_in, ResCounter<Texture>(shared_ptr<Texture>(new Texture)));
    /// Type = pair<unordered_map<string, ResCounter<Texture>>::iterator, bool>

    auto pair_it = emplace_result.first; /// unordered_map<string, ResCounter<Texture>>::iterator

    bool inserted   = emplace_result.second;
    auto tex_counter = &(pair_it->second);
    auto tex_ptr   = tex_counter->res_ptr;

    if (inserted)
    {
        /// construct the tex filepath from the key
        string filepath = basefolder + "/" + modelfolder + "/" + tex_key_in + ".obj";

        tex_ptr->fromFile(filepath);

        cout << "inserted for " << tex_key_in << "\n";
    }

    /// Important: Count another reference to the resource
    tex_counter->counter++;

    cout << "tex count for " << tex_key_in << ": " << tex_counter->counter << "\n";

    return weak_ptr<Texture>(tex_ptr);
}
