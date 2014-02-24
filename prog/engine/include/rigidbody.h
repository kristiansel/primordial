#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "object3d.h"
#include "btBulletDynamicsCommon.h"
#include "glm/glm.hpp"

class RigidBody : virtual public Object3d
{
    public:
        class Shape;
        static btCollisionShape* Box(float a, float b, float c) {return ( new btBoxShape(btVector3(a, b, c))); };
        static btCollisionShape* Sphere(float R) {return ( new btSphereShape(btScalar(R)) ); };

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

///// Shape class function objects

//class RigidBody::Shape
//{
//public:
//    virtual btCollisionShape* getShape() = 0;
//};
//
//class RigidBody::Sphere : public RigidBody::Shape
//{
//    public:
//        Sphere(float R) : r(R) {};
//        btCollisionShape* getShape() {return ( new btSphereShape(btScalar(r)) ); };
//    private:
//        float r;
//};
//
//class RigidBody::Box : public RigidBody::Shape
//{
//    public:
//        Box(float x, float y, float z) : a(x), b(y), c(z) {};
//        btCollisionShape* getShape() {return ( new btBoxShape(btVector3(a, b, c)) ); };
//    private:
//        float a, b, c;
//};



#endif // RIGIDBODY_H
