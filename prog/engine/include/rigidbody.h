#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "object3d.h"
#include "btBulletDynamicsCommon.h"
#include "glm/glm.hpp"

class RigidBody : virtual public Object3d
{
    public:
        enum class Collision {Sphere, Box, Cylinder, Plane};

        RigidBody();
        virtual ~RigidBody();

        void updateTransformation();
        void setBody(btRigidBody* body) {this->body = body; }; /// Rather make physicsworld a friend class

    protected:
    private:
        btRigidBody* body;
};

#endif // RIGIDBODY_H
