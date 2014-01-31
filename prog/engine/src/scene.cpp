#include "scene.h"

Scene::Scene()
{
    //ctor
}

Scene::~Scene()
{
    //dtor
}

string Scene::debugInfo()
{
    std::ostringstream output;
    output  << "Debugging scene: \n"
            << "camera at " << mm::stringVec3(camera.pos) << "\n"
            << "camera dir " << mm::stringVec3(camera.dir) << "\n"
//            << "mesh at " << mm::stringVec3(mesh.pos) << "\n"
//            << "mesh dir " << mm::stringVec3(mesh.dir) << "\n";
            ;

    return output.str();
}


list<Prop>::iterator Scene::addProp(string mesh_key, string tex_key, vec3 pos, vec3 dir)
//shared_ptr<Prop> Scene::addProp(string mesh_key, string tex_key, vec3 pos, vec3 dir)
{
    /// Add a new prop to the list and capture
    /// reference and iterator
    props.push_back(Prop());
    list<Prop>::iterator new_prop_it = --props.end();

    new_prop_it->pos = pos;         /// configure position
    new_prop_it->dir = dir;         /// configure direction

    /// attach the mesh
    weak_ptr<Mesh>      mesh_ptr    = resourcemanager.getMeshptrFromKey (mesh_key);
    weak_ptr<Texture>   tex_ptr     = resourcemanager.getTexptrFromKey  (tex_key);
    new_prop_it->attachBatch(mesh_ptr, tex_ptr);

    return new_prop_it;

//    return shared_ptr<Prop>(&(*new_prop_it)); /// This results in SEGFAULT because
//    /// if not captured, then this will be the last instance of this pointer, and
//    /// will automatically delete the new prop????
}

//void Scene::delProp(list<Prop>::iterator it_in)
//{
//
//}



