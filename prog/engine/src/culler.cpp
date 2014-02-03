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
    for (auto obstacle : world_in.obstacles)
    {
        scene_in.props.push_back(obstacle);
    }
}
