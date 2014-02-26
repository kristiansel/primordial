#include "mechanics.h"

Mechanics::Mechanics() : speed(2.5), camTurnSpeed(60.0)
{
    //ctor
}

Mechanics::~Mechanics()
{
    //dtor
}

void Mechanics::init(World &world_in, float &dt_in)
{
    world = &world_in;
    dt = &dt_in;

    world->camera->pos = glm::vec3(0.0, 1.5, 0.0);

    /// set the main light (sun/moon)
    world->mainLight( glm::vec3(1.0, 1.0, 1.0),         /// From direction
                      glm::vec3(1.0, 1.0, 1.0) );  /// Color

    /// add additional lights
//    world->addPointLight(glm::vec3(3.0, 1.0, -4.0),         /// Position
//                         glm::vec3(1.0, 0.5, 1.0));         /// Color
//
//    world->addPointLight(glm::vec3(-4.0, 1.0, 3.0),         /// Position
//                         glm::vec3(0.0, 1.0, 1.0));         /// Color

    /// Load some resources (should be moved)
    ///                          Model,         Texture,            Position
    /// WorldObjects:
    world->addDynamicObject( "sphere",
                             "nicewall",
                             glm::vec3(-2.0, 10.0, -4.0),
                             RigidBody::Sphere(1.0) );

    world->addDynamicObject( "cube",
                             "nicewall",
                             glm::vec3(2.0, 10.0, -4.0),
                             RigidBody::Box(0.5f, 0.5f, 0.5f) );

    world->addStaticObject( "ground",
                            "grass_equal",
                             glm::vec3(0.0, 0.0, 0.0) );

    world->addDynamicObject( "axes",
                            "tricolor",
                            glm::vec3(0.0, 8.0, 0.0),
                            RigidBody::Box(0.5f, 0.5f, 0.5f));

    /// Having issue with human model not in the correct
    /// place in the scene graph outputted by assimp import

    world->addCreature( "human_male_bgeo",
                        "tex_human_male",
                        glm::vec3(0.0, 0.0, 0.0) );

    //world->addStaticObject( "mdl_human_male",
    //                "tex_human_male",
    //                glm::vec3(0.0, 0.0, 0.0) );

    /// Creatures
    world->addCreature( "anim_test",
                        "checkers",
                        glm::vec3(0.0, 2.0, -4.0) );




    /// remove later
    worldobject_ptr_it = world->worldobjects.begin();

    /// Testing
}

void Mechanics::step(World &world_in, float dt_in)
{
    //world_in.step(dt_in);
    world_in.physicsStep(dt_in);
    for (shared_ptr<WorldObject> wObject : world_in.worldobjects)
    {
        wObject->updateTransformation();
    }
}

string Mechanics::debugInfo()
{
    std::ostringstream output;
    output  << "Debugging mechanics: \n"
            << "dt = " << *dt << "\n"
            << "speed = " << speed << "\n";
    return output.str();
}

/// move
void Mechanics::playerMoveForward()
{
    world->camera->moveForward((*dt)*speed);
}

void Mechanics::playerMoveBackward()
{
    world->camera->moveForward(-(*dt)*speed);
}

void Mechanics::playerMoveLeft()
{
    world->camera->strafeLeft((*dt)*speed);
}

void Mechanics::playerMoveRight()
{
    world->camera->strafeLeft(-(*dt)*speed);
}


/// rotate
void Mechanics::playerRotateUp()
{
    world->camera->panUp((*dt)*camTurnSpeed);
}

void Mechanics::playerRotateDown()
{
    world->camera->panUp(-(*dt)*camTurnSpeed);
}

void Mechanics::playerRotateLeft()
{
    world->camera->panLeft((*dt)*camTurnSpeed);
}

void Mechanics::playerRotateRight()
{
    world->camera->panLeft(-(*dt)*camTurnSpeed);
}

void Mechanics::func(int num_in)
{
    switch (num_in)
    {
    case 1:    /// ADD CUBE
        if (world->worldobjects.empty())
        {
            worldobject_ptr_it = world->addDynamicObject( "cube",
                                                          "nicewall",
                                                          world->camera->pos + 2.f*world->camera->getDir(),
                                                          RigidBody::Box(0.5f, 0.5f, 0.5f) );
        }
        else
        {
            world->addDynamicObject( "cube",
                                     "nicewall",
                                     world->camera->pos + 2.f*world->camera->getDir(),
                                     RigidBody::Box(0.5f, 0.5f, 0.5f)  );
        }
        break;

    case 2:     /// ADD SPHERE
        if (world->worldobjects.empty())
        {
            worldobject_ptr_it = world->addDynamicObject( "sphere",
                                                          "nicewall",
                                                          world->camera->pos + 2.f*world->camera->getDir(),
                                                          RigidBody::Sphere(1.0));
        }
        else
        {
            world->addDynamicObject( "sphere",
                                     "nicewall",
                                     world->camera->pos + 2.f*world->camera->getDir(),
                                     RigidBody::Sphere(1.0)  );
        }
        break;
    case 3:     world->delWorldObject(worldobject_ptr_it); worldobject_ptr_it = world->worldobjects.begin(); break;
    case 4:
        for (auto &creature : world->creatures)
        {
            creature->togglePauseAnim();
        }
        break;
    case 5:
        for (auto &creature : world->creatures)
        {
            if (creature->num_pose_matrices > 50)
            {
                std::cout << creature->getActiveAnimTimeMod() << "\n";
                for (int i = 0; i<4; i++)
                {
                    for (int j = 0; j<4; j++)
                    {
                        std::cout << creature->pose_matrices[4][i][j] << "\t";
                    }
                    std::cout << "\n";
                }
            }
        }
        break;

    default:    break;
    }
}
