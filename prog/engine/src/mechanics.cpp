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

    /// Load some resources (should be moved)
    ///                 Model,      Texture,    Position,               Direction
    world->addObstacle(     "sphere",   "grass_equal", vec3(0.0, 0.0, -2.0),   vec3(0.0, 0.0, -1.0));
    world->addObstacle(     "anim_test",     "checkers", vec3(3.0, 0.0, -2.0),   vec3(0.0, 0.0, -1.0));
    world->addObstacle(     "quad",     "asdasdasd", vec3(-3.0, 0.0, -2.0),   vec3(0.0, 0.0, -1.0));

    //auto prop4 = scene->addProp(     "mdl_uv_sphere",   "checkers", vec3(0.0, 0.0, 2.0),   vec3(0.0, 0.0, 1.0));
    //scene->addProp(     "quad",     "checkers", vec3(3.0, 0.0, 2.0),   vec3(0.0, 0.0, 1.0));
    //scene->addProp(     "quad",     "grass_equal", vec3(-3.0, 0.0, 2.0),   vec3(0.0, 0.0, 1.0));


    /// remove later
    obstacle_ptr_it = world->obstacles.begin();
}

void Mechanics::step(World &world_in, float dt_in)
{

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
    case 1:
        if (world->obstacles.empty())
        {
            obstacle_ptr_it = world->addObstacle(     "sphere",   "grass_equal", world->camera->pos + 2.f*world->camera->dir,   world->camera->dir);
        }
        else
        {
            world->addObstacle(     "sphere",   "grass_equal", world->camera->pos + 2.f*world->camera->dir,   world->camera->dir);
        }
        break;
    case 2:     world->delObstacle(obstacle_ptr_it); obstacle_ptr_it = world->obstacles.begin(); break;
    default:    break;
    }
}
