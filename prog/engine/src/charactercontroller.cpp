#include "charactercontroller.h"

CharacterController::CharacterController() :
    m_character(nullptr),
    m_ghostObject(nullptr)
{
    //ctor
}

CharacterController::~CharacterController()
{
    //dtor
}
void CharacterController::createCharController(btDynamicsWorld* dyn_world, glm::vec3 pos)
{
    this->dynamics_world = dyn_world;

    btTransform startTransform;
	startTransform.setIdentity ();
	startTransform.setOrigin (btVector3(pos.x,pos.y,pos.z));


	m_ghostObject = new btPairCachingGhostObject();
	m_ghostObject->setWorldTransform(startTransform);

	btScalar characterHeight= 1.90;     // 1.90 meters tall
	btScalar characterWidth = 0.5;      // radius 0.5 meters
	btConvexShape* capsule = new btCapsuleShape(characterWidth,characterHeight);
	m_ghostObject->setCollisionShape (capsule);
	// Do not set collision flags

	btScalar stepHeight = btScalar(0.35);
	m_character = new btKinematicCharacterController (m_ghostObject,capsule,stepHeight);

    dynamics_world->addCollisionObject(m_ghostObject);
	dynamics_world->addAction(m_character);
}

void CharacterController::delCharController()
{
    if (m_character)
    {
        dynamics_world->removeAction(m_character);
        dynamics_world->removeCollisionObject(m_ghostObject);

        delete m_character;
        delete m_ghostObject;
    }
}
