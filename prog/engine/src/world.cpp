#include "world.h"

World::World()  :
    camera(new Camera),
    main_light(new DirLight),
    num_point_lights(0),
    point_lights(new PointLight [MAX_NUM_POINT_LIGHTS])
{

}

World::~World()
{
    //dtor
    delete [] point_lights;
    delete main_light;
}


list<shared_ptr<WorldObject>>::iterator World::addStaticObject(string mesh_key, string tex_key, glm::vec3 pos)
//shared_ptr<WorldObject> World::addWorldObject(string mesh_key, string tex_key, glm::vec3 pos, glm::vec3 dir)
{
    /// Add a new worldobject to the list and capture
    /// reference and iterator
    shared_ptr<WorldObject> worldobject = shared_ptr<WorldObject>(new WorldObject);

    worldobject->pos = pos;         /// configure position
//    (*new_worldobject_it)->dir = dir;         /// configure direction
    addPhysicsStatic( worldobject.get() );

    /// attach the mesh
    //weak_ptr<Mesh>      mesh_ptr    = resourcemanager.getMeshptrFromKey (mesh_key);
    //weak_ptr<Texture>   tex_ptr     = resourcemanager.getTexptrFromKey  (tex_key);

    weak_ptr<Mesh>      mesh_ptr    = mesh_manager.getResptrFromKey (mesh_key);
    weak_ptr<Texture>   tex_ptr     = tex_manager.getResptrFromKey  (tex_key);

    worldobject->attachBatch(mesh_ptr, tex_ptr);

    worldobjects.push_back(worldobject);

    list<shared_ptr<WorldObject>>::iterator new_worldobject_it = --worldobjects.end();
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
    /// Create in place
//    worldobjects.push_back(shared_ptr<WorldObject>(new WorldObject()));
//    list<shared_ptr<WorldObject>>::iterator new_worldobject_it = --worldobjects.end();
    shared_ptr<WorldObject> worldobject = shared_ptr<WorldObject>(new WorldObject);

    worldobject->pos = pos;         /// configure position
//    (*new_worldobject_it)->dir = dir;         /// configure direction


    /// attach the mesh
    //weak_ptr<Mesh>      mesh_ptr    = resourcemanager.getMeshptrFromKey (mesh_key);
    //weak_ptr<Texture>   tex_ptr     = resourcemanager.getTexptrFromKey  (tex_key);

    weak_ptr<Mesh>      mesh_ptr    = mesh_manager.getResptrFromKey (mesh_key);
    weak_ptr<Texture>   tex_ptr     = tex_manager.getResptrFromKey  (tex_key);

    worldobject->attachBatch(mesh_ptr, tex_ptr);

    /// Add the collision shape if specified, if not, make a convex hull
    if (shape)
        addPhysicsDynamic( worldobject.get(), shape);
    else
        addPhysicsDynamic( worldobject.get(), RigidBody::ConvexHull(*(shared_ptr<Mesh>(mesh_ptr))));

    /// Then add it
    worldobjects.push_back(worldobject);

    /// This is not needed....
    list<shared_ptr<WorldObject>>::iterator new_worldobject_it = --worldobjects.end();
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

    /// Create completely in memory before adding it to list of
    /// renderables
    shared_ptr<Creature> creature = shared_ptr<Creature>(new Creature);

    creature->pos = pos;         /// configure position
//    (*new_creature_it)->dir = dir;         /// configure direction

    /// attach the mesh ( This is how it should be done)
    weak_ptr<Mesh>       mesh_ptr    = mesh_manager.getResptrFromKey (mesh_key);
    weak_ptr<Texture>    tex_ptr     = tex_manager.getResptrFromKey  (tex_key);

    creature->attachBatch(mesh_ptr, tex_ptr);

    /// attach skeleton
    /// Use same key for skeleton and mesh for now
    string skel_key = mesh_key;
    weak_ptr<Skeleton>   skel_ptr     = skel_manager.getResptrFromKey  (skel_key);
    creature->attachSkeleton(skel_ptr);

    /// The following bit of code is a lesson learned from another thread
    /// attempting to render the actor before it was completely loaded...
    creatures.push_back(creature);

    /// Default pose
    // creature->pose(/*anim_num=*/ 0, /*time=*/0.290000f);

//    /// Seek debug:
//    TimeSeries<float> my_time_series;
//
//    my_time_series.num_keys = 1;
//
//    my_time_series.keys = new TimeValuePair<float> [my_time_series.num_keys];
//    my_time_series.keys[0] = {0.32, 2.0};
//
//    my_time_series.duration = 1.23f;
//
//    auto res = my_time_series.seek(0.15); /// Problem!
//
//    float pre_valu = res.prev.key->value;
//    float pre_time = res.prev.key->time;
//    float pre_wght = res.prev.weight;
//    float nxt_valu = res.next.key->value;
//    float nxt_time = res.next.key->time;
//    float nxt_wght = res.next.weight;
//
//    std::cout << "prev t: " << pre_time << " v: " << pre_valu << " wt: " << pre_wght << "\n";
//    std::cout << "next t: " << nxt_time << " v: " << nxt_valu << " wt: " << nxt_wght << "\n";

    /// Start animation
    //creature->playAnim(0);

    /// Pause animation
    //creature->pauseAnim();

    /// By default, should rest-pose;
    //creature->poseRest();



    /// Debugging skeleton
    weak_ptr<Mesh>      ax_mesh_ptr    = mesh_manager.getResptrFromKey ("axes");
    weak_ptr<Texture>   ax_tex_ptr     = tex_manager.getResptrFromKey  ("tricolor");
//
//    for (int i = 0; i<creature->shSkelPtr()->num_bones; i++)
//    {
//        glm::mat4 matrix = creature->shSkelPtr()->bones[i].rest_matrix;
//        creature->attachBatch(ax_mesh_ptr, ax_tex_ptr, matrix);
//    }

//    for (int j = 0; j<6; j++)
//    {
//        creature->pose(0, 0.3);
//
//        for (int i = 0; i<creature->shSkelPtr()->num_bones; i++)
//        {
//            creature->attachBatch(ax_mesh_ptr, ax_tex_ptr, creature->pose_matrices[i]);
//        }
//    }

//
//    creature->pose(0, 0.4);
//
//    glm::mat4 orig_pos = glm::translate(glm::mat4(1.0),
//                                        glm::vec3(0.284326, 1.17341, -0.198735));
//
//    creature->attachBatch(ax_mesh_ptr, ax_tex_ptr, orig_pos);
//
//    glm::mat4* bone_mat = &(creature->pose_matrices[0]);
//    glm::vec4  bone_index(2, 0, 0, 0);
//    glm::vec4  bone_weight(1, 0, 0, 0);
//
//    glm::mat4 new_pos =
//                (bone_weight[0]*bone_mat[int(bone_index[0])]
//                +bone_weight[1]*bone_mat[int(bone_index[1])]
//                +bone_weight[2]*bone_mat[int(bone_index[2])]
//                +bone_weight[3]*bone_mat[int(bone_index[3])]) * orig_pos;

    list<shared_ptr<Creature>>::iterator new_creature_it = --creatures.end();
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

void World::mainLight(glm::vec3 dir, glm::vec4 color)
{
    main_light->dir = dir;
    main_light->color = color;
}
