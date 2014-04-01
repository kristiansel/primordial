#include "mechanics.h"

Mechanics::Mechanics() : speed(24.5), camTurnSpeed(80.0),
    controlled(nullptr),
    player(nullptr)
{

}

Mechanics::~Mechanics()
{
    //dtor
}

void Mechanics::init(World &world_in, float &dt_in)
{
    world = &world_in;
    dt = &dt_in;

    world->startMusic("lost.wav");

    // Set the input to control the camera
    //controlled = world->camera.get();
    controlled = world->freecam;

    //world->camera->pos = glm::vec3(0.0, 1.5, 0.0);
    world->freecam->pos = glm::vec3(0.0, 1.5, 0.0);

    // set the main light (sun/moon)
    world->mainLight( glm::vec3(1.0, 1.0, 1.0),         // From direction
                      glm::vec4(1.0, 1.0, 1.0, 1.0) );  // Color

    // add additional lights
//    world->addPointLight(glm::vec3(3.0, 1.0, -4.0),         // Position
//                         glm::vec3(1.0, 0.5, 1.0));         // Color
//
//    world->addPointLight(glm::vec3(-4.0, 1.0, 3.0),         // Position
//                         glm::vec3(0.0, 1.0, 1.0));         // Color

    // Load some resources (should be moved)
    //                          Model,         Texture,            Position
    // WorldObjects:


    world->addDynamicObject( "sphere",
                             "nicewall",
                             glm::vec3(-2.0, 10.0, -4.0),
                             RigidBody::Sphere(1.0) );


//    world->addDynamicObject( "rabbit",
//                             "rabbit_d",
//                             glm::vec3(2.0, 10.0, -4.0));

    world->addDynamicObject( "loin_x3",
                             "nicewall",
                             glm::vec3(2.0, 10.0, -4.0));


    // sword... forgot to apply mirror modifiers.. :)
//    world->addDynamicObject( "sword_x",
//                             "nicewall",
//                             glm::vec3(2.0, 10.0, -4.0));

    world->addStaticObject( "quad",
                            "grass_equal",
                             glm::vec3(0.0, 0.0, 0.0) );

    world->addStaticObject( "terrain",
                            "grass_equal",
                             glm::vec3(0.0, 0.0, 0.0) );

    world->addDynamicObject( "axes",
                            "tricolor",
                            glm::vec3(0.0, 8.0, 0.0));

    // Add player
    {
//        auto playerCreature = world->addCreature( "human_version2",
        auto playerCreature = world->addCreature( "human_all_anim3",
                            "tex_human_male",
                            glm::vec3(3.0, 0.0, 2.0) );

        // Set player to point to the first added creature
        player = (*playerCreature).get();

        // Set chase_cam to be behind the player
        world->chasecam->pos = player->pos - 5.f * player->getLookDir() + glm::vec3(0.0, 2.0, 0.0); // Hard camera
        world->chasecam->rot = player->getLookRot(); // Hard camera

        // set the player as the receiver of control signals
        controlled = player;

        // set the active camera as the chase cam
        world->active_cam = world->chasecam;
    }

    // attach a loincloth for modesty
    {
        weak_ptr<Mesh>      mesh_ptr    = global::mesh_manager.getResptrFromKey ("loin_x3");
        weak_ptr<Texture>   tex_ptr     = global::tex_manager.getResptrFromKey  ("tex_knapsack");

        player->attachBatch(mesh_ptr, tex_ptr);
    }

    // give a sword
    {
        weak_ptr<Mesh>      mesh_ptr    = global::mesh_manager.getResptrFromKey ("sword_03");
//        weak_ptr<Mesh>      mesh_ptr    = global::mesh_manager.getResptrFromKey ("axes");
        weak_ptr<Texture>   tex_ptr     = global::tex_manager.getResptrFromKey  ("nicewall");
//        weak_ptr<Texture>   tex_ptr     = global::tex_manager.getResptrFromKey  ("tricolor");

        RenderBatch* sword_batch = player->attachBatch(mesh_ptr, tex_ptr);
        player->moveBatchToSlot(sword_batch, Actor::Slot::RightHand);
    }

    // add another creature
    Creature* other = nullptr;
    {
        auto otherCreature = world->addCreature( "human_all_anim3",
                            "tex_human_male",
                            glm::vec3(-3.0, 0.0, 2.0) );

        other = (*otherCreature).get();
    }

    // attach a loincloth
    {
        weak_ptr<Mesh>      mesh_ptr    = global::mesh_manager.getResptrFromKey ("loin_x3");
        weak_ptr<Texture>   tex_ptr     = global::tex_manager.getResptrFromKey  ("tex_knapsack");

        other->attachBatch(mesh_ptr, tex_ptr);
    }

    // give a sword
    {
        weak_ptr<Mesh>      mesh_ptr    = global::mesh_manager.getResptrFromKey ("sword_03");
        weak_ptr<Texture>   tex_ptr     = global::tex_manager.getResptrFromKey  ("nicewall");

        RenderBatch* sword_batch = other->attachBatch(mesh_ptr, tex_ptr);
        other->moveBatchToSlot(sword_batch, Actor::Slot::RightHand);

//        // Item* item::sword
//
//        // ideally
//        player->acquire(item::sword[4]);            // acquire a specific sword
//        player->acquire(item::sword);               // acquire a random sword
//        player->acquire(item::sword(parameters));   // find an appropriate sword based on parameters
//        player->acquire(item::sword, slot::backpack); // place it it backpack
//        player->acquire(item::sword, slot::offhand); // pick it up with offhand
//        player->acquire(item::sword, slot::lefthip); // place it on left hip
//
//        // for now
//        player->acquire(item::sword);               // acquire the only sword in the game right now
//
//        // then to equip
//        player->equip(item::sword); // main hand default
//        player->equip(item::sword, slot::mainhand);
//        player->equip(item::sword, slot::offhand);
//         // find the best alternative for mainhand (given skills and what is available)
//        player->equip(slot::mainhand);
//        player->draw(slot::mainhand);
//        player->draw(slot::offhand);

    }




//    world->addCreature( "humale_1hswing",
//                        "tex_human_male",
//                        glm::vec3(-3.0, 0.0, 2.0) );
//
//    // Creatures
//    world->addCreature( "anim_test",
//                        "checkers",
//                        glm::vec3(0.0, 2.0, -4.0) );

    world->addDynamicObject( "cube",
                             "nicewall",
                             glm::vec3(2.0, 10.0, -4.0),
                             RigidBody::Box(0.5f, 0.5f, 0.5f) );


    // remove later
    worldobject_ptr_it = world->worldobjects.begin();

    // Testing
}

void Mechanics::step(World &world_in, float dt_in)
{
    // Resolve creature signals
    for (shared_ptr<Creature> creature : world_in.creatures)
    {
        creature->resolveActionRequests(dt_in);
    }

    // step physics
    world_in.physicsStep(dt_in);

    // Update transforms

    // update object transforms
    for (shared_ptr<WorldObject> wObject : world_in.worldobjects)
    {
        wObject->updateTransformation();
    }

    // update creature transforms
    for (shared_ptr<Creature> creature : world_in.creatures)
    {
        creature->updateTransformation();
    }



    // Make the chase cam chase the player
    world->chasecam->pos = player->pos - 5.f * player->getLookDir() + glm::vec3(0.0, 2.0, 0.0); // Hard camera
    world->chasecam->rot = player->getLookRot(); // Hard camera

//    float cam_dist = 5.0;
//    world->chasecam->pos = player->pos - cam_dist * player->getLookDir();
//    world->chasecam->
}

string Mechanics::debugInfo()
{
    std::ostringstream output;
    output  << "Debugging mechanics: \n"
            << "dt = " << *dt << "\n"
            << "speed = " << speed << "\n";
    return output.str();
}

// move
void Mechanics::playerMoveForward()
{
    if (controlled) controlled->moveForward((*dt)*speed, *dt);
}

void Mechanics::playerMoveBackward()
{
    if (controlled) controlled->moveForward(-(*dt)*speed, *dt);
}

void Mechanics::playerMoveLeft()
{
    if (controlled) controlled->moveLeft((*dt)*speed, *dt);
}

void Mechanics::playerMoveRight()
{
    if (controlled) controlled->moveLeft(-(*dt)*speed, *dt);
}


// rotate
void Mechanics::playerRotateUp()
{
    if (controlled) controlled->rotateUp((*dt)*camTurnSpeed, *dt);
}

void Mechanics::playerRotateDown()
{
    if (controlled) controlled->rotateUp(-(*dt)*camTurnSpeed, *dt);
}

void Mechanics::playerRotateLeft()
{
    if (controlled) controlled->rotateLeft((*dt)*camTurnSpeed, *dt);
}

void Mechanics::playerRotateRight()
{
    if (controlled) controlled->rotateLeft(-(*dt)*camTurnSpeed, *dt);
}

void Mechanics::playerRotateUpVal(float val)
{
    if (controlled) controlled->rotateUp(val, *dt);
}

void Mechanics::playerRotateLeftVal(float val)
{
    if (controlled) controlled->rotateLeft(val, *dt);
}

void Mechanics::playerAttack()
{
    if (controlled) controlled->attack();
}

void Mechanics::playerDodge()
{
    if (controlled) controlled->dodge();
}

void Mechanics::playerBlock()
{
    if (controlled) controlled->block();
}

void Mechanics::playerShift()
{
    if (controlled) controlled->shift();
}

void Mechanics::playerJump()
{
    if (controlled) controlled->jump();
}

void Mechanics::playerStance()
{
    if (controlled) controlled->stance();
}

void Mechanics::func(int num_in)
{
    switch (num_in)
    {
    case 1:    // ADD CUBE
        if (world->worldobjects.empty())
        {
              worldobject_ptr_it = world->addDynamicObject( "rabbit",
                                                          "rabbit_d",
                                                          world->freecam->pos + 2.f*world->freecam->getDir());
                                                          //world->camera->pos + 2.f*world->camera->getDir());
        }
        else
        {
              worldobject_ptr_it = world->addDynamicObject( "rabbit",
                                                          "rabbit_d",
                                                          world->freecam->pos + 2.f*world->freecam->getDir());
                                                          //world->camera->pos + 2.f*world->camera->getDir());
        }
        break;

    case 2:     // ADD SPHERE
        if (world->worldobjects.empty())
        {
            worldobject_ptr_it = world->addDynamicObject( "sphere",
                                                          "nicewall",
                                                          world->freecam->pos + 2.f*world->freecam->getDir(),
                                                          /*world->camera->pos + 2.f*world->camera->getDir(),*/
                                                          RigidBody::Sphere(1.0));
        }
        else
        {
            world->addDynamicObject( "sphere",
                                     "nicewall",
                                     world->freecam->pos + 2.f*world->freecam->getDir(),
                                     /*world->camera->pos + 2.f*world->camera->getDir(),*/
                                     RigidBody::Sphere(1.0)  );
        }
        break;
    case 3:     world->delWorldObject(worldobject_ptr_it); worldobject_ptr_it = world->worldobjects.begin(); break;
    case 4:
        for (auto &creature : world->creatures)
        {
            creature->togglePauseAnim(); // This is inefficient, seeing as Idle animation is
            // "played" all the time
        }
        break;
    case 5:
        for (auto &creature : world->creatures)
        {
            if (creature->getActiveAnimIndex() < (creature->getNumAnims()-1))
            {
                creature->playAnim(creature->getActiveAnimIndex()+1);
            }
            else
            {
                creature->playAnim(0);
            }
        }
        break;
    case 6: // switch between free-cam and chase cam
        if (controlled == world->freecam)
        {
            if (world->creatures.size() > 0)
            {
                // set player as controlled
                controlled = player;

                // set chase cam as active
                world->active_cam = world->chasecam;
            }
        }
        else
        {
            // set the freecam as controlled
            controlled = world->freecam;

            // set the freecam as active
            world->active_cam = world->freecam;

            // make the freecam duplicate the chasecam
            world->freecam->pos = world->chasecam->pos;
            world->freecam->rot = world->chasecam->rot;
        }
        break;
//    case 7:
//        if (world->active_cam == world->freecam)
//            world->active_cam = world->chasecam;
//        else
//            world->active_cam = world->freecam;
//
//
//        break;

    default:    break;
    }
}
