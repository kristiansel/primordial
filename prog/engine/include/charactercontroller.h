#ifndef CHARACTERCONTROLLER_H
#define CHARACTERCONTROLLER_H

#include "object3d.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btMultiSphereShape.h"
//#include "BulletDynamics/Character/btKinematicCharacterController.h"
//#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "glm/glm.hpp"

/// The following bit of code is grabbed from bullet on github

class DynamicCharacterController
{
protected:
//        btScalar m_halfHeight;
        btCollisionShape* m_shape;
        btRigidBody* m_rigidBody;
//
//        btVector3 m_raySource[2];
//        btVector3 m_rayTarget[2];
//        btScalar m_rayLambda[2];
//        btVector3 m_rayNormal[2];
//
//        btScalar m_turnAngle;
//
//        btScalar m_maxLinearVelocity;
//        btScalar m_walkVelocity;
//        btScalar m_turnVelocity;
public:
        DynamicCharacterController ();
        ~DynamicCharacterController ();
        void setup (btDynamicsWorld* dynamicsWorld, btScalar height, btScalar width, /*btScalar stepHeight,*/ glm::vec3 pos);
        void destroy (btDynamicsWorld* dynamicsWorld);

        void setVelocity(glm::vec3 v);
        void applyForce(glm::vec3 f);
        void velocitySetpoint(glm::vec3 v_s);
        glm::vec3 getWorldPos() const;

private:
        // Gain/how fast can you change your speed (maybe this agi)
        float gain;         // (N /(m/s)), should make this a function of strength/agi and mass

        // maximum force able to exert (maybe this strength)
        float f_max;        // N, should make this a function of strength/agi and mass

//        btCollisionObject* getCollisionObject ();
//
//        void preStep (btDynamicsWorld* dynamicsWorld);
//        void playerStep (btScalar dt,
//                                         int forward,
//                                         int backward,
//                                         int left,
//                                         int right,
//                                         int jump);
//        bool canJump () const;
//        void jump ();
//
//        bool onGround () const;
};






/// OLD IMPLEMENTATION BELOW
//   |
//   |
//   |
//   |
//   |
//   |
//   V


// Make it kinematic for now (does not get pushed back by the world)
//
//class CharacterController
//{
//    public:
//        CharacterController();
//        virtual ~CharacterController();
//
//        void createCharController(btDynamicsWorld *dyn_world,
//                                  btBroadphaseInterface *pair_cache,
//                                  glm::vec3 pos);
//        void delCharController();
//        void displace(const glm::vec3 &displacement);
//
//        void updateTransform(glm::vec3 &pos, glm::quat &rot); // use motionstate for this..
//
//    protected:
//    private:
////        btKinematicCharacterController* m_character;
////        class btPairCachingGhostObject* m_ghostObject;
////
////        // The dynamics world that this object belongs to
//
//        btRigidBody* m_rigidBody;
//        btDynamicsWorld *dynamics_world;
//
//
//
//
//};

#endif // CHARACTERCONTROLLER_H
