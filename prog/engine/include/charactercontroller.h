#ifndef CHARACTERCONTROLLER_H
#define CHARACTERCONTROLLER_H

#include "object3d.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btMultiSphereShape.h"
//#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "glm/glm.hpp"
#include "geometry.h"

/// The following bit of code is grabbed from bullet on github

class DynamicCharacterController
{
protected:
        // For the body
        btCollisionShape* m_shape;
        btRigidBody* m_rigidBody;

        // Threat region (for attacking)
        btCollisionShape* m_threat_shape;
        btGhostObject* m_threat_object;


public:
        DynamicCharacterController ();

        ~DynamicCharacterController ();

        void setup (btDynamicsWorld* dynamicsWorld_in,
                    btScalar height, btScalar radius, glm::vec3 pos, btScalar mass);

        void destroy ();

        void setVelocity(glm::vec3 v);

        void applyForce(glm::vec3 f);

        void jump(glm::vec3 forw);

        void lunge(glm::vec3 forw);

        void velocitySetpoint(glm::vec3 v_s);

        void applyMoveController();

        void testThreatRegion();

        void updateThreatRegionTransf(glm::vec3 pos, glm::quat rot);

        //void setUser(void *user);

        glm::vec3 getWorldPos() const;

        bool onGround();

        struct HitInfo
        {
            bool isHit;
            float hitStrength;
        };

        HitInfo* getHitInfo();

private:
        // Gain/how fast can you change your speed (maybe this agi)
        float gain;         // (N /(m/s)), should make this a function of strength/agi and mass

        // maximum force able to exert (maybe this strength)
        float f_max;        // N, should make this a function of strength/agi and mass

        btVector3 f_set;

        float m_halfHeight;


        // temporary storage
        btVector3 m_rayStart;
        btVector3 m_rayEnd;

        HitInfo m_hitInfo;


        bool on_ground;

        btDynamicsWorld* m_dynamicsWorld;
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
