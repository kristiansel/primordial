#ifndef CULLER_H
#define CULLER_H

#include "world.h"
#include "scene.h"

/// Later, make some fancy way of deciding what to send for rendering
/// for now this class just makes the master.scene up of everything in the master.world

class Culler
{
    public:
        Culler();
        virtual ~Culler();

        void stage(Scene &scene_in, World &world_in);
    protected:
    private:
};

#endif // CULLER_H
