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

        // Update position
        pos = glm::vec3(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ())  );

        // Update rotation
        btQuaternion bt_rot = trans.getRotation();
        rot = glm::quat(float(bt_rot.w()), float(bt_rot.x()), float(bt_rot.y()), float(bt_rot.z()) ) ;
    }
}

RigidBody::~RigidBody()
{

}
