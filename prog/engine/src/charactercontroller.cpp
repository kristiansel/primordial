#include "charactercontroller.h"

/// The following bit of code is grabbed from bullet on github

DynamicCharacterController::DynamicCharacterController () :
    gain(8000.0),
    f_max(30000.0), // 20000 too low. this will only limit "getting off the ground"
    m_halfHeight(0.0),
    on_ground(false),
    f_set(btVector3(0.0, 0.0, 0.0))
{
        m_shape = nullptr;
        m_rigidBody = nullptr;
}

DynamicCharacterController::~DynamicCharacterController ()
{
}

void DynamicCharacterController::setup (btDynamicsWorld* dynamicsWorld, btScalar height, btScalar radius, glm::vec3 pos, btScalar mass)
{
    m_dynamicsWorld = dynamicsWorld;

    radius = (radius > 0.0) ? radius : -radius ;                                   // must be positive
    height = (height > 0.0) ? height : -height ;                                 // must be positive
    height = (height > 2.f*radius) ? height : radius*2.00001;     // height must be > radius

    m_halfHeight = height/2.0;

    m_shape = new btCapsuleShape (radius, height-2*radius);


//height+2*radius

    btTransform startTransform;
    startTransform.setIdentity ();
    startTransform.setOrigin (btVector3(pos.x, pos.y+m_halfHeight, pos.z));
    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

    btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, m_shape);
    m_rigidBody = new btRigidBody(cInfo);

    m_rigidBody->setSleepingThresholds (0.0, 0.0);
    m_rigidBody->setAngularFactor (0.0);
    m_dynamicsWorld->addRigidBody (m_rigidBody);

    btVector3 comPosition = m_rigidBody->getCenterOfMassPosition();
    glm::vec3 out = glm::vec3((float)comPosition[0], (float)comPosition[1], (float)comPosition[2]);
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



