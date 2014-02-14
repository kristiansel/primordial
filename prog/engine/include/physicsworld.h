#ifndef PHYSICSWORLD_H
#define PHYSICSWORLD_H

#include "btBulletDynamicsCommon.h"
#include "rigidbody.h"
#include "glm/glm.hpp"


class PhysicsWorld
{
    public:
        PhysicsWorld();
        virtual ~PhysicsWorld();

        void physicsStep(float dt_in);

    protected:
        /// Should aim for something like
        /// addPhysicsObject(CollShape shape, float mass, glm::vec3 position)
        /// addPhysicsObject(Sphere(1.0), 1.0, glm::vec3(1.0, 2.0, -1.0))
        // void addPhysicsObject(RigidBody::Collision shape, par1=0, par2=0, par3=0, par4=0);
        void addPhysicsDynamic(RigidBody* rigidbody, btCollisionShape* shape);
        void addPhysicsStatic(RigidBody* rigidbody);

        void removePhysicsObject(RigidBody* rigidbody);

    private:
        ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
        btDefaultCollisionConfiguration* collisionConfiguration;

        ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
        btCollisionDispatcher* dispatcher;

        ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
        btBroadphaseInterface* overlappingPairCache;

        ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
        btSequentialImpulseConstraintSolver* solver;

        btDiscreteDynamicsWorld* dynamicsWorld;

        btAlignedObjectArray<btCollisionShape*> collisionShapes;

};

#endif // PHYSICSWORLD_H
