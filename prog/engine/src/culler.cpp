#include "culler.h"

Culler::Culler()
{
    // ctor
}

Culler::~Culler()
{
    // dtor
}

void Culler::stage(Scene &scene_in, World &world_in)
{
    scene_in.camera = world_in.camera;

    scene_in.props.clear(); /// rebuild the render list
    scene_in.actors.clear(); /// rebuild the render list

    for (auto obstacle : world_in.obstacles)
    {
        scene_in.props.push_back(obstacle);
    }

    for (auto creature : world_in.creatures)
    {
        scene_in.actors.push_back(creature);
    }
}
