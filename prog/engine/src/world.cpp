#include "world.h"

World::World()  : camera(new Camera)
{

}

World::~World()
{
    //dtor
}


list<shared_ptr<WorldObject>>::iterator World::addStaticObject(string mesh_key, string tex_key, glm::vec3 pos)
//shared_ptr<WorldObject> World::addWorldObject(string mesh_key, string tex_key, glm::vec3 pos, glm::vec3 dir)
{
    /// Add a new worldobject to the list and capture
    /// reference and iterator
    worldobjects.push_back(shared_ptr<WorldObject>(new WorldObject()));
    list<shared_ptr<WorldObject>>::iterator new_worldobject_it = --worldobjects.end();

    (*new_worldobject_it)->pos = pos;         /// configure position
//    (*new_worldobject_it)->dir = dir;         /// configure direction
    addPhysicsStatic( (*new_worldobject_it).get() );

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


//list<shared_ptr<WorldObject>>::iterator World::addDynamicObject(string mesh_key, string tex_key, glm::vec3 pos)
////shared_ptr<WorldObject> World::addWorldObject(string mesh_key, string tex_key, glm::vec3 pos, glm::vec3 dir)
//{
//    /// Add a new worldobject to the list and capture
//    /// reference and iterator
//    worldobjects.push_back(shared_ptr<WorldObject>(new WorldObject()));
//    list<shared_ptr<WorldObject>>::iterator new_worldobject_it = --worldobjects.end();
//
//    (*new_worldobject_it)->pos = pos;         /// configure position
////    (*new_worldobject_it)->dir = dir;         /// configure direction
//    addPhysicsDynamic( (*new_worldobject_it).get() );
//
//    /// attach the mesh
//    //weak_ptr<Mesh>      mesh_ptr    = resourcemanager.getMeshptrFromKey (mesh_key);
//    //weak_ptr<Texture>   tex_ptr     = resourcemanager.getTexptrFromKey  (tex_key);
//
//    weak_ptr<Mesh>      mesh_ptr    = mesh_manager.getResptrFromKey (mesh_key);
//    weak_ptr<Texture>   tex_ptr     = tex_manager.getResptrFromKey  (tex_key);
//
//    (*new_worldobject_it)->attachBatch(mesh_ptr, tex_ptr);
//
//    return new_worldobject_it;
//
////    return shared_ptr<WorldObject>(&(*new_worldobject_it)); /// This results in SEGFAULT because
////    /// if not captured, then this will be the last instance of this pointer, and
////    /// will automatically delete the new worldobject????
//}
        /// WorldObject is defined by:
        /// Model               GFX             string/enum
        /// Texture             GFX             string/enum
        /// Material            GFX             (Material)
        /// position            GFX/PHY         vec3
        /// rotation            GFX/PHY         (quat <- should be otional)
        /// scale               GFX             (vec3 <- consider )
        /// Collision shape     PHY             enum

list<shared_ptr<WorldObject>>::iterator World::addDynamicObject(string mesh_key,
                                                                string tex_key,
                                                                glm::vec3 pos,
                                                                btCollisionShape* shape)
//shared_ptr<WorldObject> World::addWorldObject(string mesh_key, string tex_key, glm::vec3 pos, glm::vec3 dir)
{
    /// Add a new worldobject to the list and capture
    /// reference and iterator
    worldobjects.push_back(shared_ptr<WorldObject>(new WorldObject()));
    list<shared_ptr<WorldObject>>::iterator new_worldobject_it = --worldobjects.end();

    (*new_worldobject_it)->pos = pos;         /// configure position
//    (*new_worldobject_it)->dir = dir;         /// configure direction
    addPhysicsDynamic( (*new_worldobject_it).get(), shape);

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
    if (!worldobjects.empty())
    {
        /// assume worldobject_it_in is a valid iterator
        removePhysicsObject( (*worldobject_it_in).get() );

        worldobjects.erase(worldobject_it_in);
    }
}

list<shared_ptr<Creature>>::iterator World::addCreature(string mesh_key, string tex_key, glm::vec3 pos)
//shared_ptr<Creature> World::addCreature(string mesh_key, string tex_key, glm::vec3 pos, glm::vec3 dir)
{
    /// Add a new creature to the list and capture
    /// reference and iterator
    creatures.push_back(shared_ptr<Creature>(new Creature()));
    list<shared_ptr<Creature>>::iterator new_creature_it = --creatures.end();

    (*new_creature_it)->pos = pos;         /// configure position
//    (*new_creature_it)->dir = dir;         /// configure direction

    /// load bones and animation
    (*new_creature_it)->fromFile("../tools/primordial_asset_importer/test_anims.bbns");

            /// For debugging: Add the bones as renderbatches instead of model

            glm::mat4* matrices = new glm::mat4 [(*new_creature_it)->num_bones];

            /// void Skeleton::poseMatrices(glm::mat4* matrices, int anim_index, float time);
            (*new_creature_it)->poseMatrices(matrices, 0, 0.0);

            for (int i = 0; i<(*new_creature_it)->num_bones; i++)
            {
                weak_ptr<Mesh>      mesh_ptr    = mesh_manager.getResptrFromKey ("axes");
                weak_ptr<Texture>   tex_ptr     = tex_manager.getResptrFromKey  ("tricolor");
                glm::mat4 matrix = (*new_creature_it)->bones[i].rest_matrix;

                (*new_creature_it)->attachBatch(mesh_ptr, tex_ptr, matrices[i]);
                (*new_creature_it)->attachBatch(mesh_ptr, tex_ptr, matrix);
            } /// Next up, make interpolation/keyframe animations

            delete[] matrices;

    /// attach the mesh ( This is how it should be done)
    weak_ptr<Mesh>      mesh_ptr    = mesh_manager.getResptrFromKey (mesh_key);
    weak_ptr<Texture>   tex_ptr     = tex_manager.getResptrFromKey  (tex_key);
////    weak_ptr<Mesh>      ax_mesh_ptr    = mesh_manager.getResptrFromKey ("axes");
////    weak_ptr<Texture>   ax_tex_ptr     = tex_manager.getResptrFromKey  ("tricolor");
//
//
    (*new_creature_it)->attachBatch(mesh_ptr, tex_ptr);
//    (*new_creature_it)->attachBatch(ax_mesh_ptr, ax_tex_ptr);


    return new_creature_it;

//    return shared_ptr<Creature>(&(*new_creature_it)); /// This results in SEGFAULT because
//    /// if not captured, then this will be the last instance of this pointer, and
//    /// will automatically delete the new creature????
}

void World::delCreature(list<shared_ptr<Creature>>::iterator creature_it_in)
{
    if (!creatures.empty()) creatures.erase(creature_it_in);
}

//void World::step(float dt_in)
//{
//    physicsStep(dt_in);
//    for (shared_ptr<WorldObject> wObject : worldobjects)
//    {
//        wObject->updateTransformation();
//    }
//}
