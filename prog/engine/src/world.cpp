#include "world.h"

World::World()  : camera(new Camera)
{

}

World::~World()
{
    //dtor
}

list<shared_ptr<Obstacle>>::iterator World::addObstacle(string mesh_key, string tex_key, vec3 pos, vec3 dir)
//shared_ptr<Obstacle> World::addObstacle(string mesh_key, string tex_key, vec3 pos, vec3 dir)
{
    /// Add a new obstacle to the list and capture
    /// reference and iterator
    obstacles.push_back(shared_ptr<Obstacle>(new Obstacle()));
    list<shared_ptr<Obstacle>>::iterator new_obstacle_it = --obstacles.end();

    (*new_obstacle_it)->pos = pos;         /// configure position
    (*new_obstacle_it)->dir = dir;         /// configure direction

    /// attach the mesh
    //weak_ptr<Mesh>      mesh_ptr    = resourcemanager.getMeshptrFromKey (mesh_key);
    //weak_ptr<Texture>   tex_ptr     = resourcemanager.getTexptrFromKey  (tex_key);

    weak_ptr<Mesh>      mesh_ptr    = mesh_manager.getResptrFromKey (mesh_key);
    weak_ptr<Texture>   tex_ptr     = tex_manager.getResptrFromKey  (tex_key);

    (*new_obstacle_it)->attachBatch(mesh_ptr, tex_ptr);

    return new_obstacle_it;

//    return shared_ptr<Obstacle>(&(*new_obstacle_it)); /// This results in SEGFAULT because
//    /// if not captured, then this will be the last instance of this pointer, and
//    /// will automatically delete the new obstacle????
}

void World::delObstacle(list<shared_ptr<Obstacle>>::iterator obstacle_it_in)
{
    if (!obstacles.empty()) obstacles.erase(obstacle_it_in);
}
