#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "prop.h"
#include "rigidbody.h"

class WorldObject : public Prop, public RigidBody
// class WorldObject : public Prop
{
    public:
        WorldObject();
        virtual ~WorldObject();
    protected:
    private:
};

#endif // OBSTACLE_H
