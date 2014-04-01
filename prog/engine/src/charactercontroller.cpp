#include "charactercontroller.h"

/// The following bit of code is grabbed from bullet on github

DynamicCharacterController::DynamicCharacterController () :
    gain(8000.0),
    f_max(30000.0), // 20000 too low. this will only limit "getting off the ground"
    m_halfHeight(0.0),
    on_ground(false),
    f_set(btVector3(0.0, 0.0, 0.0)),
    m_hitInfo({false, 0.0})
{
        m_shape = nullptr;
        m_rigidBody = nullptr;

        m_threat_shape = nullptr;
        m_threat_object = nullptr;
}

DynamicCharacterController::~DynamicCharacterController ()
{
}

void DynamicCharacterController::setup (btDynamicsWorld* dynamicsWorld, btScalar height, btScalar radius, glm::vec3 pos, btScalar mass)
{
    m_dynamicsWorld = dynamicsWorld;


    { // the character capsule shape
        radius = (radius > 0.0) ? radius : -radius ;                                   // must be positive
        height = (height > 0.0) ? height : -height ;                                 // must be positive
        height = (height > 2.f*radius) ? height : radius*2.00001;     // height must be > radius

        m_halfHeight = height/2.0;

        m_shape = new btCapsuleShape (radius, height-2*radius);

        btTransform startTransform;
        startTransform.setIdentity ();
        startTransform.setOrigin (btVector3(pos.x, pos.y+m_halfHeight, pos.z));
        btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

        btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, m_shape);
        //cInfo.m_friction = 0.2f;
        m_rigidBody = new btRigidBody(cInfo);

        m_rigidBody->setSleepingThresholds (0.0, 0.0);
        m_rigidBody->setAngularFactor (0.0);
        m_rigidBody->setUserPointer(&m_hitInfo);

        m_dynamicsWorld->addRigidBody (m_rigidBody, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter);
    }

    { // Threat shape
        // threat shape (a box in front of character that represents melee/touch range)
        // 2x2x2 m box;
        m_threat_shape = new btBoxShape (btVector3(0.30, 0.5, 0.5));

        btGhostPairCallback* ghostCall = new btGhostPairCallback();
        dynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(ghostCall);

        m_threat_object = new btGhostObject();

        m_threat_object->setCollisionShape(m_threat_shape);
        btTransform trans;
        trans.setIdentity();
        trans.setOrigin(btVector3(pos.x, pos.y+1.0f, pos.z-1.0f));
        m_threat_object->setWorldTransform(trans);
        m_threat_object->setCollisionFlags(m_threat_object->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
        //dynamicsWorld->addCollisionObject(m_threat_object, btBroadphaseProxy::SensorTrigger, btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::SensorTrigger);
        dynamicsWorld->addCollisionObject(m_threat_object, btBroadphaseProxy::SensorTrigger, btBroadphaseProxy::CharacterFilter);

        //dynamicsWorld->addCollisionObject(m_threat_object);
    }

}

void DynamicCharacterController::updateThreatRegionTransf(glm::vec3 pos, glm::quat rot)
{
    btTransform trans;
    trans.setIdentity();
    trans.setRotation(btQuaternion(rot.w, rot.x, rot.y, rot.z));
    trans.setOrigin(btVector3(pos.x, pos.y, pos.z));

    m_threat_object->setWorldTransform(trans);
}


void DynamicCharacterController::destroy ()
{
        if (m_shape)
        {
                delete m_shape;
        }

        if (m_rigidBody)
        {
                m_dynamicsWorld->removeRigidBody (m_rigidBody);
                delete m_rigidBody;
        }

        if (m_threat_shape)
        {
                delete m_threat_shape;
        }

        if (m_threat_object)
        {
                m_dynamicsWorld->removeCollisionObject(m_threat_object);
                delete m_threat_object;
        }
}

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
        btVector3 v = m_rigidBody->getLinearVelocity();

        btVector3 v_s = btVector3(glm_v_s.x, glm_v_s.y, glm_v_s.z);

        // Simple P(ID) controller
        f_set = gain*(v_s-v);
        btScalar f_mag = f_set.length();

        f_set = (f_mag > f_max) ? f_set/f_max : f_set ;

        //std::cout << "applying force " << f.length() << " N \n";

        //                             cannot control y (cannot fly)


}
void DynamicCharacterController::applyMoveController()
{
    if (on_ground)
    {
        m_rigidBody->applyCentralForce(btVector3(f_set[0], 0.f, f_set[2]));
    }
}

DynamicCharacterController::HitInfo *DynamicCharacterController::getHitInfo()
{
    return &m_hitInfo;
}

void DynamicCharacterController::testThreatRegion()
{
    //std::cout << "testing threat region" << std::endl;
    if (m_threat_object)
    {
        int num_overlapping = m_threat_object->getNumOverlappingObjects();
        for (int i = 0; i<num_overlapping; i++)
        {
            btCollisionObject* col_object = m_threat_object->getOverlappingObject(i);

            if (col_object != m_rigidBody) // Do not count hits with self
            {
                HitInfo* user = (HitInfo*) col_object->getUserPointer();
                if (user)
                {
                    *user = {true, 10.0}; // set some more funny hit thing here...
                }
                else std::cerr << "error: testThreatRegion() hit collision object has no user\n";
            }
            else
            {
                //std::cout << "registered hit with self\n";
            }

        }

        //std::cout << "overlapping: " << num_overlapping << std::endl;
    }
}


glm::vec3 DynamicCharacterController::getWorldPos() const
{
    btVector3 comPosition = m_rigidBody->getCenterOfMassPosition();
    glm::vec3 out = glm::vec3((float)comPosition[0], (float)comPosition[1], (float)comPosition[2]);

    out = out - glm::vec3(0.0, m_halfHeight, 0.0);


    return out;
}

void DynamicCharacterController::jump(glm::vec3 forw)
{
    if (on_ground)
    {
        btVector3 forwbt = btVector3(forw.x, forw.y, forw.z);
        btVector3 up(0.0, 1.0, 0.0);
        btScalar magnitude = (btScalar(1.0)/m_rigidBody->getInvMass()) * btScalar(4.5);
        m_rigidBody->applyCentralImpulse ((up + forwbt*0.7)* magnitude);
    }
}

void DynamicCharacterController::lunge(glm::vec3 forw)
{
    if (on_ground)
    {
        btVector3 forwbt = btVector3(forw.x, forw.y, forw.z);
        btScalar magnitude = (btScalar(1.0)/m_rigidBody->getInvMass()) * btScalar(4.7);
        m_rigidBody->applyCentralImpulse (forwbt* magnitude);
    }
}

bool DynamicCharacterController::onGround()
{
    m_rayStart = m_rigidBody->getCenterOfMassPosition();
    m_rayEnd = m_rayStart - btVector3(0.0, m_halfHeight*1.05, 0.0);

    // rayCallback
    btCollisionWorld::AllHitsRayResultCallback rayCallback(m_rayStart, m_rayEnd);

    m_dynamicsWorld->rayTest (m_rayStart, m_rayEnd, rayCallback);
    if (rayCallback.hasHit())
    {
        on_ground = true;
        return true;
    }
    on_ground = false;
    return false;
}



