#include "charactercontroller.h"

/// The following bit of code is grabbed from bullet on github

DynamicCharacterController::DynamicCharacterController () :
    gain(8000.0),
    f_max(30000.0) // 20000 too low. this will only limit "getting off the ground"
{
//        m_rayLambda[0] = 1.0;
//        m_rayLambda[1] = 1.0;
//        m_halfHeight = 1.0;
//        m_turnAngle = 0.0;
//        m_maxLinearVelocity = 10.0;
//        m_walkVelocity = 8.0; // meters/sec
//        m_turnVelocity = 1.0; // radians/sec
        m_shape = nullptr;
        m_rigidBody = nullptr;
}

DynamicCharacterController::~DynamicCharacterController ()
{
}

void DynamicCharacterController::setup (btDynamicsWorld* dynamicsWorld, btScalar height, btScalar width, /*btScalar stepHeight,*/ glm::vec3 pos)
{
        btVector3 spherePositions[2];
        btScalar sphereRadii[2];

        sphereRadii[0] = width;
        sphereRadii[1] = width;
        spherePositions[0] = btVector3 (0.0, (height/btScalar(2.0) - width), 0.0);
        spherePositions[1] = btVector3 (0.0, (-height/btScalar(2.0) + width), 0.0);

//        m_halfHeight = height/btScalar(2.0);

        /// Newer version of bullet:
        //m_shape = new btMultiSphereShape (btVector3(width/btScalar(2.0), height/btScalar(2.0), width/btScalar(2.0)), &spherePositions[0], &sphereRadii[0], 2);

        /// Old version of bullet
        m_shape = new btMultiSphereShape (&spherePositions[0], &sphereRadii[0], 2);

        btTransform startTransform;
        startTransform.setIdentity ();
        startTransform.setOrigin (btVector3(pos.x, pos.y, pos.z));
        btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

        btScalar mass = 80.0;

        btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, m_shape);
        m_rigidBody = new btRigidBody(cInfo);
        // kinematic vs. static doesn't work
        //m_rigidBody->setCollisionFlags( m_rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
        m_rigidBody->setSleepingThresholds (0.0, 0.0);
        m_rigidBody->setAngularFactor (0.0);
        dynamicsWorld->addRigidBody (m_rigidBody);
}

void DynamicCharacterController::destroy (btDynamicsWorld* dynamicsWorld)
{
        if (m_shape)
        {
                delete m_shape;
        }

        if (m_rigidBody)
        {
                dynamicsWorld->removeRigidBody (m_rigidBody);
                delete m_rigidBody;
        }
}
//
//btCollisionObject* DynamicCharacterController::getCollisionObject ()
//{
//        return m_rigidBody;
//}

//void DynamicCharacterController::preStep (btDynamicsWorld* dynamicsWorld)
//{
//        btTransform xform;
//        m_rigidBody->getMotionState()->getWorldTransform (xform);
//        btVector3 down = -xform.getBasis()[1];
//        btVector3 forward = xform.getBasis()[2];
//        down.normalize ();
//        forward.normalize();
//
//        m_raySource[0] = xform.getOrigin();
//        m_raySource[1] = xform.getOrigin();
//
//        m_rayTarget[0] = m_raySource[0] + down * m_halfHeight * btScalar(1.1);
//        m_rayTarget[1] = m_raySource[1] + forward * m_halfHeight * btScalar(1.1);
//
//        class ClosestNotMe : public btCollisionWorld::ClosestRayResultCallback
//        {
//        public:
//                ClosestNotMe (btRigidBody* me) : btCollisionWorld::ClosestRayResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0))
//                {
//                        m_me = me;
//                }
//
//                virtual btScalar AddSingleResult(btCollisionWorld::LocalRayResult& rayResult,bool normalInWorldSpace)
//                {
//                        if (rayResult.m_collisionObject == m_me)
//                                return 1.0;
//
//                        return ClosestRayResultCallback::AddSingleResult (rayResult, normalInWorldSpace
//                );
//        }
//        protected:
//                btRigidBody* m_me;
//        };
//
//        ClosestNotMe rayCallback(m_rigidBody);
//
//        int i = 0;
//        for (i = 0; i < 2; i++)
//        {
//                rayCallback.m_closestHitFraction = 1.0;
//                dynamicsWorld->rayTest (m_raySource[i], m_rayTarget[i], rayCallback);
//                if (rayCallback.HasHit())
//                {
//                        m_rayLambda[i] = rayCallback.m_closestHitFraction;
//                } else {
//                        m_rayLambda[i] = 1.0;
//                }
//        }
//}

//void DynamicCharacterController::playerStep (btScalar dt,
//                                         int forward,
//                                         int backward,
//                                         int left,
//                                         int right,
//                                         int jump)
//
//{
//        btTransform xform;
//        m_rigidBody->getMotionState()->getWorldTransform (xform);
//
//        /* Handle turning */
//        if (left)
//                m_turnAngle -= dt * m_turnVelocity;
//        if (right)
//                m_turnAngle += dt * m_turnVelocity;
//
//        xform.setRotation (btQuaternion (btVector3(0.0, 1.0, 0.0), m_turnAngle));
//
//        btVector3 linearVelocity = m_rigidBody->getLinearVelocity();
//        btScalar speed = m_rigidBody->getLinearVelocity().length();
//
//        btVector3 forwardDir = xform.getBasis()[2];
//        forwardDir.normalize ();
//        btVector3 walkDirection = btVector3(0.0, 0.0, 0.0);
//        btScalar walkSpeed = m_walkVelocity * dt;
//
//        if (forward)
//                walkDirection += forwardDir;
//        if (backward)
//                walkDirection -= forwardDir;
//
//
//
//        if (!forward && !backward && onGround())
//        {
//                /* Dampen when on the ground and not being moved by the player */
//                linearVelocity *= btScalar(0.2);
//                m_rigidBody->setLinearVelocity (linearVelocity);
//        } else {
//                if (speed < m_maxLinearVelocity)
//                {
//                        btVector3 velocity = linearVelocity + walkDirection * walkSpeed;
//                        m_rigidBody->setLinearVelocity (velocity);
//                }
//        }
//
//        m_rigidBody->getMotionState()->setWorldTransform (xform);
//        m_rigidBody->setCenterOfMassTransform (xform);
//}

void DynamicCharacterController::setVelocity(glm::vec3 v)
{
    m_rigidBody->setLinearVelocity(btVector3(v.x, v.y, v.z));
}

void DynamicCharacterController::applyForce(glm::vec3 f)
{
    m_rigidBody->applyCentralForce(btVector3(f.x, f.y, f.z));
}

void DynamicCharacterController::velocitySetpoint(glm::vec3 glm_v_s)
{
    //float gain = 12000.0;
    //float gain = 8000.0;
    //float gain = 4000.0;
    btVector3 v = m_rigidBody->getLinearVelocity();

    btVector3 v_s = btVector3(glm_v_s.x, glm_v_s.y, glm_v_s.z);

    // Simple P(ID) controller
    btVector3 f = gain*(v_s-v);
    btScalar f_mag = f.length();

    f = (f_mag > f_max) ? f/f_max : f ;

    //std::cout << "applying force " << f.length() << " N \n";

    //                             cannot control y (cannot fly)
    m_rigidBody->applyCentralForce(btVector3(f[0], 0.f, f[2]));
}


glm::vec3 DynamicCharacterController::getWorldPos() const
{
    btVector3 comPosition = m_rigidBody->getCenterOfMassPosition();
    glm::vec3 out = glm::vec3((float)comPosition[0], (float)comPosition[1], (float)comPosition[2]);
    out = out-glm::vec3(0.0, 1.9/2.0, 0.0);
    return out;
}

//bool DynamicCharacterController::canJump () const
//{
//        return onGround();
//}
//
//void DynamicCharacterController::jump ()
//{
//        if (!canJump())
//                return;
//
//        btTransform xform;
//        m_rigidBody->getMotionState()->getWorldTransform (xform);
//        btVector3 up = xform.getBasis()[1];
//        up.normalize ();
//        btScalar magnitude = (btScalar(1.0)/m_rigidBody->getInvMass()) * btScalar(8.0);
//        m_rigidBody->applyCentralImpulse (up * magnitude);
//}
//
//bool DynamicCharacterController::onGround () const
//{
//        return m_rayLambda[0] < btScalar(1.0);
//}

/// OLD IMPLEMENTATION BELOW
//   |
//   |
//   |
//   |
//   |
//   |
//   V



//
//CharacterController::CharacterController() :
////    m_character(nullptr),
////    m_ghostObject(nullptr)
//    m_rigidBody(nullptr),
//    dynamics_world(nullptr)
//{
//    //ctor
//}
//
//CharacterController::~CharacterController()
//{
//    //dtor
//}
//
//void CharacterController::createCharController(btDynamicsWorld* dyn_world,
//                                               btBroadphaseInterface *pair_cache,
//                                               glm::vec3 pos)
//{
////    this->dynamics_world = dyn_world;
////
////    btTransform startTransform;
////	startTransform.setIdentity ();
////	startTransform.setOrigin (btVector3(pos.x,pos.y,pos.z));
////
////
////	m_ghostObject = new btPairCachingGhostObject();
////	m_ghostObject->setWorldTransform(startTransform);
////
////	// WHAT?
////	pair_cache->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
////
////	btScalar characterHeight= 1.90;     // 1.90 meters tall
////	btScalar characterWidth = 0.5;      // radius 0.5 meters
////	btConvexShape* capsule = new btCapsuleShape(characterWidth,characterHeight);
////	m_ghostObject->setCollisionShape (capsule);
////	m_ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT); // WHAT?
////
////	btScalar stepHeight = btScalar(0.35);
////	m_character = new btKinematicCharacterController (m_ghostObject,capsule,stepHeight);
////
////    dynamics_world->addCollisionObject(m_ghostObject, // Add ghost object
////                                       btBroadphaseProxy::CharacterFilter, // WHAT?
////                                       btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter); // WHAT?
////
////	dynamics_world->addAction(m_character);
//}
//
//void CharacterController::delCharController()
//{
////    if (m_character)
////    {
////        dynamics_world->removeAction(m_character);
////        dynamics_world->removeCollisionObject(m_ghostObject);
////
////        delete m_character;
////        delete m_ghostObject;
////    }
//}
//
//void CharacterController::displace(const glm::vec3 &d)
//{
//    // It seems a bit strange, but in the example they use walkDirection*walkSpeed*dt
//    // as argument, so I assume that using walk-displacement is correct
//    //m_character->setWalkDirection(btVector3(d.x, d.y, d.z));
//
//
//
//
//
//}
//
//void CharacterController::updateTransform(glm::vec3 &pos, glm::quat &rot)
//{
////    btTransform trans;
////    m_ghostObject->getWorldTransform(trans);
////
////    // Update position
////    pos = glm::vec3(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ())  );
////
////    // Update rotation
////    btQuaternion bt_rot = trans.getRotation();
////    rot = glm::quat(float(bt_rot.w()), float(bt_rot.x()), float(bt_rot.y()), float(bt_rot.z()) ) ;
//}
