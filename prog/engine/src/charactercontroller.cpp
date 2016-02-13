#include "charactercontroller.h"

/// The following bit of code is grabbed from bullet on github

DynamicCharacterController::DynamicCharacterController () :
    gain(8000.0),
    f_max(5000.0), // 20000 too low. this will only limit "getting off the ground"
    f_set(btVector3(0.0, 0.0, 0.0)),
    v_set(btVector3(0.0, 0.0, 0.0)),
    m_halfHeight(0.0),
    on_ground(false),
    m_footingNormal(glm::vec3(0.0, 1.0, 0.0))
{
        m_shape = nullptr;
        m_rigidBody = nullptr;

        m_threat_shape = nullptr;
        m_threat_object = nullptr;

//        m_ghostCall = nullptr;
}

DynamicCharacterController::~DynamicCharacterController ()
{
    destroy();
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
//    rbInfo.m_restitution = btScalar(0.99f);
        cInfo.m_friction = btScalar(0.0f);
        m_rigidBody = new btRigidBody(cInfo);

        m_rigidBody->setSleepingThresholds (0.0, 0.0);
        m_rigidBody->setAngularFactor (0.0);
        //m_rigidBody->setUserPointer(&m_hitInfo);

        m_dynamicsWorld->addRigidBody (m_rigidBody, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter);
    }

    { // Threat shape
        // threat shape (a box in front of character that represents melee/touch range)
        // 2x2x2 m box;
        m_threat_shape = new btBoxShape (btVector3(0.30, 0.5, 0.5));

        //m_ghostCall = new btGhostPairCallback(); // why is this even here?
        //dynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(m_ghostCall); // why?

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
            m_shape = nullptr;
    }

    if (m_rigidBody)
    {
            auto motionState = m_rigidBody->getMotionState(); // experimental
            if (motionState) delete motionState; // experimental: works fine..

            m_dynamicsWorld->removeRigidBody (m_rigidBody);
            delete m_rigidBody;
            m_rigidBody = nullptr;
    }

    if (m_threat_shape)
    {
            delete m_threat_shape;
            m_threat_shape = nullptr;
    }

    //if (m_ghostCall) delete m_ghostCall;

    if (m_threat_object)
    {
            m_dynamicsWorld->removeCollisionObject(m_threat_object);
            delete m_threat_object;
            m_threat_object = nullptr;
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
//        btVector3 v = m_rigidBody->getLinearVelocity();
//
//        btVector3 v_s = btVector3(glm_v_s.x, glm_v_s.y, glm_v_s.z);
//
//        // Simple P(ID) controller
//        f_set = gain*(v_s-v);
//        btScalar f_mag = f_set.length();
//
//        f_set = (f_mag > f_max) ? f_set/f_max : f_set ;
//
//        //std::cout << "applying force " << f.length() << " N \n";
//
//        //                             cannot control y (cannot fly)
    //f_set = btVector3(glm_v_s.x, glm_v_s.y, glm_v_s.z);
    v_set = btVector3(glm_v_s.x, glm_v_s.y, glm_v_s.z);

}
void DynamicCharacterController::applyMoveController()
{
    // This function gets used all the time
    if (on_ground)
    {
        //float force_scale = 100.f;

        // apply P(ID) controller
        btVector3 v_meas = m_rigidBody->getLinearVelocity();
        btVector3 v_diff = v_meas - v_set;

        float gain = -10000.f;
        btVector3 force = gain*v_diff;

        // limit the force to the allowable range
        float force_abs_mag = std::abs(force.length());

        float force_rat = force_abs_mag/f_max;
        if (force_rat > 1.f)
        {
            force = force/force_rat;
        }

        m_rigidBody->applyCentralForce(btVector3(force[0], 0.f, force[2]));

        //m_rigidBody->applyCentralForce(btVector3(force_scale*f_set[0], 0.f, force_scale*f_set[2]));

        //
        //std::cout << "on ground\n";


        //m_rigidBody->setLinearVelocity(btVector3(f_set[0], (m_rigidBody->getLinearVelocity())[1], f_set[2]));

        //float y_adj = (-m_footingNormal.x*f_set[0] - m_footingNormal.z*f_set[2])/m_footingNormal.y;
//
//        m_rigidBody->setLinearVelocity(btVector3(f_set[0],
//                                                 (m_rigidBody->getLinearVelocity())[1],
//                                                 f_set[2]));
        //std::cout << "ON ground\n";
    }

}

void DynamicCharacterController::setVelocityXZ(float v_x, float v_z)
{
    // This function is unused
    if (on_ground)
    {
        m_rigidBody->setLinearVelocity(btVector3(v_x, (m_rigidBody->getLinearVelocity())[1], v_z));

        //std::cout << "getting here\n";
    }
}

void DynamicCharacterController::getThreatened(void** &targets,
                                               unsigned int &num_targets,
                                               unsigned int max_num_targets)
{
    //std::cout << "testing threat region" << std::endl;
    if (m_threat_object)
    {
        int num_overlapping = m_threat_object->getNumOverlappingObjects();
        num_targets = 0;
        for (int i = 0; (i<num_overlapping && num_targets<max_num_targets); i++)
        {
            btCollisionObject* col_object = m_threat_object->getOverlappingObject(i);

            if (col_object != m_rigidBody) // Do not count hits with self
            {
                auto user = col_object->getUserPointer();
                if (user)
                {
                    targets[num_targets] = user;
                    num_targets++;
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

void DynamicCharacterController::setUser(void* pointerIn)
{
    m_rigidBody->setUserPointer(pointerIn);
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
        //btVector3 forwbt = btVector3(forw.x, forw.y, forw.z);
        btVector3 up(0.0, 1.0, 0.0);
        btScalar magnitude = (btScalar(1.0)/m_rigidBody->getInvMass()) * btScalar(4.5);
        //m_rigidBody->applyCentralImpulse ((up + forwbt*0.7)* magnitude);
        m_rigidBody->applyCentralImpulse (up * magnitude);
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
//    btVector3 vel = m_rigidBody->getLinearVelocity();
//
//    btVector3 dir = vel.normalized();
//
//    float climbing_factor = 0.16;
//
//    m_rayStart = m_rigidBody->getCenterOfMassPosition();
//    m_rayEnd = m_rayStart - btVector3(dir[0]*climbing_factor, m_halfHeight*(1+climbing_factor/2.0), dir[2]*climbing_factor);

    m_rayStart = m_rigidBody->getCenterOfMassPosition();
    m_rayEnd = m_rayStart - btVector3(0.0, m_halfHeight*(1.2), 0.0);

    // rayCallback
    btCollisionWorld::ClosestRayResultCallback rayCallback(m_rayStart, m_rayEnd);

    m_dynamicsWorld->rayTest (m_rayStart, m_rayEnd, rayCallback);
    if (rayCallback.hasHit())
    {
        on_ground = true;
        m_footingNormal = glm::vec3(rayCallback.m_hitNormalWorld[0], rayCallback.m_hitNormalWorld[1], rayCallback.m_hitNormalWorld[2]);
        return true;
    }
    on_ground = false;
    return false;
}



