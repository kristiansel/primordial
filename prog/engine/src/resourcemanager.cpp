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
//    auto emplace_result = meshes.emplace(mesh_key_in, ResCounter<Mesh>(shared_ptr<Mesh>(new Mesh)));
    auto emplace_result = meshes.emplace(mesh_key_in, shared_ptr<Mesh>(new Mesh)); /// THIS CALLS MESH DESTRUCTOR IF ALREADY THERE
//    auto pair_it = emplace_result.first;
    auto mesh_it    = emplace_result.first;
    bool inserted   = emplace_result.second;
//    auto mesh_counter = pair_it->second;
//    auto mesh_ptr   = mesh_counter.res_ptr;
    auto mesh_ptr   = mesh_it->second;

    if (inserted)
    {
        /// construct the mesh filepath from the key
        string filepath = basefolder + "/" + modelfolder + "/" + mesh_key_in + ".obj";

        mesh_ptr->fromFile(filepath);
    }

    /// Important: Count another reference to the resource
//    mesh_counter.counter++; /// Could instead just check shared_ptr::unique on each unload?
    cout << "usecount for " << mesh_key_in << " is " << mesh_ptr.use_count() << "\n";
    return weak_ptr<Mesh>(mesh_ptr);
}

weak_ptr<sf::Texture> ResourceManager::getTexptrFromKey(string tex_key_in)      /// TEXTURE
{
    auto emplace_result = textures.emplace(tex_key_in, shared_ptr<sf::Texture>(new sf::Texture));
    auto tex_it    = emplace_result.first;
    bool inserted  = emplace_result.second;
    auto tex_ptr   = tex_it->second;

    if (inserted)
    {
        /// construct the mesh filepath from the key
        string filepath = basefolder + "/" + texturefolder + "/" + tex_key_in + ".obj";

        tex_ptr->loadFromFile(filepath);
    }

    return weak_ptr<sf::Texture>(tex_ptr);
}

