#include "rigidbody.h"

RigidBody::RigidBody() : body(nullptr)
{
    //ctor
}

void RigidBody::updateTransformation()
{
    if (body && body->getMotionState())
    {
        btTransform trans;
        body->getMotionState()->getWorldTransform(trans);
        // printf("world pos = %f,%f,%f\n",float(trans.getOrigin().getX()),float(trans.getOrigin().getY()),float(trans.getOrigin().getZ()));
        pos = vec3(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ())  );
    }
}

RigidBody::~RigidBody()
{
    //dtor
}
