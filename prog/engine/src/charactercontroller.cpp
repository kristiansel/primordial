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
void CharacterController::createCharController(btDynamicsWorld* dyn_world,
                                               btBroadphaseInterface *pair_cache,
                                               glm::vec3 pos)
{
    this->dynamics_world = dyn_world;

    btTransform startTransform;
	startTransform.setIdentity ();
	startTransform.setOrigin (btVector3(pos.x,pos.y,pos.z));


	m_ghostObject = new btPairCachingGhostObject();
	m_ghostObject->setWorldTransform(startTransform);

	// WHAT?
	pair_cache->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

	btScalar characterHeight= 1.90;     // 1.90 meters tall
	btScalar characterWidth = 0.5;      // radius 0.5 meters
	btConvexShape* capsule = new btCapsuleShape(characterWidth,characterHeight);
	m_ghostObject->setCollisionShape (capsule);
	m_ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT); // WHAT?

	btScalar stepHeight = btScalar(0.35);
	m_character = new btKinematicCharacterController (m_ghostObject,capsule,stepHeight);

    dynamics_world->addCollisionObject(m_ghostObject, // Add ghost object
                                       btBroadphaseProxy::CharacterFilter, // WHAT?
                                       btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter); // WHAT?

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

void CharacterController::displace(const glm::vec3 &d)
{
    // displacement = (vec3)direction * (float)speed * (float)dt

//    ///set walkDirection for our character
//    btTransform xform;
//    xform = m_ghostObject->getWorldTransform ();
//
//    btVector3 forwardDir = xform.getBasis()[2];
////	printf("forwardDir=%f,%f,%f\n",forwardDir[0],forwardDir[1],forwardDir[2]);
//    btVector3 upDir = xform.getBasis()[1];
//    btVector3 strafeDir = xform.getBasis()[0];
//    forwardDir.normalize ();
//    upDir.normalize ();
//    strafeDir.normalize ();
//
//    btVector3 walkDirection = btVector3(0.0, 0.0, 0.0);
//    btScalar walkVelocity = btScalar(1.1) * 4.0; // 4 km/h -> 1.1 m/s
//    btScalar walkSpeed = walkVelocity * dt;
//
//    //rotate view
//    if (gLeft)
//    {
//        btMatrix3x3 orn = m_ghostObject->getWorldTransform().getBasis();
//        orn *= btMatrix3x3(btQuaternion(btVector3(0,1,0),0.01));
//        m_ghostObject->getWorldTransform ().setBasis(orn);
//    }
//
//    if (gRight)
//    {
//        btMatrix3x3 orn = m_ghostObject->getWorldTransform().getBasis();
//        orn *= btMatrix3x3(btQuaternion(btVector3(0,1,0),-0.01));
//        m_ghostObject->getWorldTransform ().setBasis(orn);
//    }
//
//    if (gForward)
//        walkDirection += forwardDir;
//
//    if (gBackward)
//        walkDirection -= forwardDir;



    // It seems a bit strange, but in the example they use walkDirection*walkSpeed*dt
    // as argument, so I assume that using walk-displacement is correct
    m_character->setWalkDirection(btVector3(d.x, d.y, d.z));





}

void CharacterController::updateTransform(glm::vec3 &pos, glm::quat &rot)
{
//    btTransform trans;
//    m_ghostObject->getWorldTransform(trans);
//
//    // Update position
//    pos = glm::vec3(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ())  );
//
//    // Update rotation
//    btQuaternion bt_rot = trans.getRotation();
//    rot = glm::quat(float(bt_rot.w()), float(bt_rot.x()), float(bt_rot.y()), float(bt_rot.z()) ) ;
}
