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

        /// There is some rotation going on. It is not correct, but....
        btQuaternion bt_rot = trans.getRotation();
        Quat rot_quat = Quat(float(bt_rot.w()), float(bt_rot.x()), float(bt_rot.y()), float(bt_rot.z()) );
        vec4 big_dir =  Transform::quatRot(rot_quat) * vec4(0.0, 0.0, -1.0, 0.0);
        dir = vec3(big_dir.x, big_dir.y, big_dir.z);
    }
}

RigidBody::~RigidBody()
{

}
