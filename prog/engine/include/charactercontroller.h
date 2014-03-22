#ifndef CHARACTERCONTROLLER_H
#define CHARACTERCONTROLLER_H

#include "object3d.h"
#include "btBulletDynamicsCommon.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "glm/glm.hpp"

// Make it kinematic for now (does not get pushed back by the world)

class CharacterController
{
    public:
        CharacterController();
        virtual ~CharacterController();

        void createCharController(btDynamicsWorld *dyn_world,
                                  btBroadphaseInterface *pair_cache,
                                  glm::vec3 pos);
        void delCharController();
        void displace(const glm::vec3 &displacement);

        void updateTransform(glm::vec3 &pos, glm::quat &rot); // use motionstate for this..

    protected:
    private:
        btKinematicCharacterController* m_character;
        class btPairCachingGhostObject* m_ghostObject;

        // The dynamics world that this object belongs to
        btDynamicsWorld *dynamics_world;


};

#endif // CHARACTERCONTROLLER_H
