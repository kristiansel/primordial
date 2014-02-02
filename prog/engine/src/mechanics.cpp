#include "mechanics.h"

Mechanics::Mechanics() : speed(2.5), camTurnSpeed(60.0)
{
    //ctor
}

Mechanics::~Mechanics()
{
    //dtor
}

void Mechanics::init(Scene &scene_in, float &dt_in)
{
    scene = &scene_in;
    dt = &dt_in;

    /// Load some resources (should be moved)
    ///                 Model,      Texture,    Position,               Direction
    scene->addProp(     "mdl_uv_sphere",   "grass_equal", vec3(0.0, 0.0, -2.0),   vec3(0.0, 0.0, -1.0));
    scene->addProp(     "quad",     "checkers", vec3(3.0, 0.0, -2.0),   vec3(0.0, 0.0, -1.0));
    scene->addProp(     "asdasdas",     "checkers", vec3(-3.0, 0.0, -2.0),   vec3(0.0, 0.0, -1.0));

    //auto prop4 = scene->addProp(     "mdl_uv_sphere",   "checkers", vec3(0.0, 0.0, 2.0),   vec3(0.0, 0.0, 1.0));
    //scene->addProp(     "quad",     "checkers", vec3(3.0, 0.0, 2.0),   vec3(0.0, 0.0, 1.0));
    //scene->addProp(     "quad",     "grass_equal", vec3(-3.0, 0.0, 2.0),   vec3(0.0, 0.0, 1.0));


    /// remove later
    prop_it = scene->props.begin();
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
    scene->camera.moveForward((*dt)*speed);
}

void Mechanics::playerMoveBackward()
{
    scene->camera.moveForward(-(*dt)*speed);
}

void Mechanics::playerMoveLeft()
{
    scene->camera.strafeLeft((*dt)*speed);
}

void Mechanics::playerMoveRight()
{
    scene->camera.strafeLeft(-(*dt)*speed);
}


/// rotate
void Mechanics::playerRotateUp()
{
    scene->camera.panUp((*dt)*camTurnSpeed);
}

void Mechanics::playerRotateDown()
{
    scene->camera.panUp(-(*dt)*camTurnSpeed);
}

void Mechanics::playerRotateLeft()
{
    scene->camera.panLeft((*dt)*camTurnSpeed);
}

void Mechanics::playerRotateRight()
{
    scene->camera.panLeft(-(*dt)*camTurnSpeed);
}

void Mechanics::func(int num_in)
{
    switch (num_in)
    {
    case 1:     scene->addProp(     "mdl_uv_sphere",   "grass_equal", scene->camera.pos + 2.f*scene->camera.dir,   scene->camera.dir); break;
    case 2:     scene->delProp(prop_it); prop_it = scene->props.begin(); break;
    default:    break;
    }
}
