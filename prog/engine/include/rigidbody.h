#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "object3d.h"
#include "btBulletDynamicsCommon.h"
#include "glm/glm.hpp"

class RigidBody : virtual public Object3d
{
    public:
        enum class Collision {Sphere, Box, Cylinder, Plane}; /// Not used so far

        RigidBody();
        virtual ~RigidBody();

        void updateTransformation();
        void setBody(btRigidBody* body) {this->body = body; }; /// Rather make physicsworld a friend class
        btRigidBody* getBody() {return body;}; /// Rather make physicsworld a friend class

    protected:
    private:
        btRigidBody* body; /// Non-owning

        /// Mass is contained in the bullet API...
};

#endif // RIGIDBODY_H
