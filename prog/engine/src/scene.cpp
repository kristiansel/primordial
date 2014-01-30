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


list<Prop>::iterator Scene::addProp(string mesh_key, vec3 pos, vec3 dir)
{
    /// Add a new prop to the list and capture
    /// reference and iterator
    props.push_back(Prop());
    Prop &new_prop = props.back();
    list<Prop>::iterator prop_it = --props.end();

    new_prop.pos = pos;         /// configure position
    new_prop.dir = dir;         /// configure direction

    /// attach the mesh
    weak_ptr<Mesh> mesh_ptr = resourcemanager.getMeshptrFromKey(mesh_key);
    new_prop.attachMesh(mesh_ptr);


}

//void Scene::delProp(list<Prop>::iterator it_in)
//{
//
//}



