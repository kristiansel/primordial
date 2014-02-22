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

    /// NB this also adds the ground
    world->addDynamicObject( "axes",
                            "tricolor",
                            glm::vec3(0.0, 8.0, 0.0),
                            RigidBody::Box(0.5f, 0.5f, 0.5f));

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
            /// dump bones to cout
            for (int i_bone = 0; i_bone<creature->num_pose_matrices; i_bone++)
            {
                std::cout<<"bone matrix "<<i_bone<<"\n";
                std::cout<<"";
                for (int i = 0; i<4; i++)
                {
                    std::cout<<"";
                    for (int j = 0; j<4; j++)
                    {
                        std::cout << creature->pose_matrices[i_bone][j][i];
                        if(j<3)std::cout<<",";
                    }
                    std::cout<<"";
                    if(i<3)std::cout<<"\n";
                }
                std::cout<<"\n\n";
            }
        }
        break;

    default:    break;
    }
}
