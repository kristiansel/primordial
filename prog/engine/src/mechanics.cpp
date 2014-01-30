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
    ///                 Model,      Position,               Direction
    scene->addProp(     "sphere",   vec3(0.0, 0.0, -2.0),   vec3(0.0, 0.0, -1.0));
    scene->addProp(     "quad",     vec3(3.0, 0.0, -2.0),   vec3(0.0, 0.0, -1.0));
    scene->addProp(     "quad",     vec3(-3.0, 0.0, -2.0),   vec3(0.0, 0.0, -1.0));
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
