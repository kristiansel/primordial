#include "world.h"

World::World()  : camera(new Camera)
{

}

World::~World()
{
    //dtor
}

list<shared_ptr<WorldObject>>::iterator World::addWorldObject(string mesh_key, string tex_key, vec3 pos, vec3 dir)
//shared_ptr<WorldObject> World::addWorldObject(string mesh_key, string tex_key, vec3 pos, vec3 dir)
{
    /// Add a new worldobject to the list and capture
    /// reference and iterator
    worldobjects.push_back(shared_ptr<WorldObject>(new WorldObject()));
    list<shared_ptr<WorldObject>>::iterator new_worldobject_it = --worldobjects.end();

    (*new_worldobject_it)->pos = pos;         /// configure position
    (*new_worldobject_it)->dir = dir;         /// configure direction
    addPhysicsObject( (*new_worldobject_it).get() );

    /// attach the mesh
    //weak_ptr<Mesh>      mesh_ptr    = resourcemanager.getMeshptrFromKey (mesh_key);
    //weak_ptr<Texture>   tex_ptr     = resourcemanager.getTexptrFromKey  (tex_key);

    weak_ptr<Mesh>      mesh_ptr    = mesh_manager.getResptrFromKey (mesh_key);
    weak_ptr<Texture>   tex_ptr     = tex_manager.getResptrFromKey  (tex_key);

    (*new_worldobject_it)->attachBatch(mesh_ptr, tex_ptr);

    return new_worldobject_it;

//    return shared_ptr<WorldObject>(&(*new_worldobject_it)); /// This results in SEGFAULT because
//    /// if not captured, then this will be the last instance of this pointer, and
//    /// will automatically delete the new worldobject????
}

void World::delWorldObject(list<shared_ptr<WorldObject>>::iterator worldobject_it_in)
{
    if (!worldobjects.empty()) worldobjects.erase(worldobject_it_in);
}

list<shared_ptr<Creature>>::iterator World::addCreature(string mesh_key, string tex_key, vec3 pos, vec3 dir)
//shared_ptr<Creature> World::addCreature(string mesh_key, string tex_key, vec3 pos, vec3 dir)
{
    /// Add a new creature to the list and capture
    /// reference and iterator
    creatures.push_back(shared_ptr<Creature>(new Creature()));
    list<shared_ptr<Creature>>::iterator new_creature_it = --creatures.end();

    (*new_creature_it)->pos = pos;         /// configure position
    (*new_creature_it)->dir = dir;         /// configure direction

    /// attach the mesh
    weak_ptr<Mesh>      mesh_ptr    = mesh_manager.getResptrFromKey (mesh_key);
    weak_ptr<Texture>   tex_ptr     = tex_manager.getResptrFromKey  (tex_key);

    (*new_creature_it)->attachBatch(mesh_ptr, tex_ptr);
//    (*new_creature_it)->loadFromBNS("assets_raw/skeletons/" + mesh_key + ".bns"); /// Move this to a resourcemanager as well

    return new_creature_it;

//    return shared_ptr<Creature>(&(*new_creature_it)); /// This results in SEGFAULT because
//    /// if not captured, then this will be the last instance of this pointer, and
//    /// will automatically delete the new creature????
}

void World::delCreature(list<shared_ptr<Creature>>::iterator creature_it_in)
{
    if (!creatures.empty()) creatures.erase(creature_it_in);
}

void World::step(float dt_in)
{
    physicsStep(dt_in);
    for (shared_ptr<WorldObject> wObject : worldobjects)
    {
        wObject->updateTransformation();
    }
}
