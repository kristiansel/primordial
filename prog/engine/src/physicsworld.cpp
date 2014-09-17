#include "physicsworld.h"

PhysicsWorld::PhysicsWorld()
{
	//collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collisionConfiguration = new btDefaultCollisionConfiguration();

	//use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new	btCollisionDispatcher(collisionConfiguration);

	//btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	//overlappingPairCache = new btDbvtBroadphase();
	overlappingPairCache = new btAxisSweep3(btVector3(-1000,-1000,-1000), btVector3(1000,1000,1000));

	//the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;

    // Initialize the world
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,solver,collisionConfiguration);

    // Set gravity (have to be a little bit accurate :)
    { // MUTEX: DO NOT REALLY NEED HERE, BUT IT DOESN'T HURT
        PrimT::LockGuard(PhysMutex::dynworld_mx);
        dynamicsWorld->setGravity(btVector3(0,-9.81,0));

        // create debug drawer
        debugDraw = new DebugDrawer;

        // attach debug draw
        dynamicsWorld->setDebugDrawer(debugDraw);
	}
}

// void PhysicsWorld::addPhysicsObject(RigidBody::Collision shape, par1=0, par2=0, par3=0, par4=0)
void PhysicsWorld::addPhysicsDynamic(RigidBody* rigidbody, btCollisionShape* shape)
{
    //create a dynamic rigidbody

    //btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
    // btCollisionShape* colShape = new btSphereShape(btScalar(1.));
    btCollisionShape* colShape = shape;
    {
        PrimT::LockGuard guard(PhysMutex::col_shap_mx);
        collisionShapes.push_back(colShape);
    }

    // Create Dynamic Objects
    btTransform startTransform;
    startTransform.setIdentity();

    btScalar	mass(10.f);

    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0,0,0);
    if (isDynamic)
        colShape->calculateLocalInertia(mass,localInertia);

    // Set starting rotation
    btQuaternion bt_start_quat = btQuaternion(rigidbody->rot.w, rigidbody->rot.x,
                                              rigidbody->rot.y, rigidbody->rot.z);
    startTransform.setRotation( bt_start_quat ) ;

    // Set starting translation
    startTransform.setOrigin(btVector3(rigidbody->pos.x, rigidbody->pos.y, rigidbody->pos.z));


    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
    rbInfo.m_restitution = btScalar(0.33f);
    rbInfo.m_friction = btScalar(0.4f);
    btRigidBody* body = new btRigidBody(rbInfo);

    rigidbody->setBody(body);

    { // MUTEX
        PrimT::LockGuard(PhysMutex::dynworld_mx);
        dynamicsWorld->addRigidBody(body);
    }
}

void PhysicsWorld::addPhysicsStatic(RigidBody* rigidbody, btCollisionShape* shape)
{
    //create a dynamic rigidbody

    //btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
    // I think this puts the plane at -1 y?
    btCollisionShape* colShape = shape;
    shape->setLocalScaling(btVector3(rigidbody->scale.x, rigidbody->scale.y, rigidbody->scale.z) );

    { // MUTEX
        PrimT::LockGuard guard(PhysMutex::col_shap_mx);
        collisionShapes.push_back(colShape); // NEED MUTEX
    }

    // Create Dynamic Objects
    btTransform startTransform;
    startTransform.setIdentity();

    // Set scale
    btScalar	mass(0.f);

    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0,0,0);
    if (isDynamic)
        colShape->calculateLocalInertia(mass,localInertia);

   // Set starting rotation
    btQuaternion bt_start_quat = btQuaternion(rigidbody->rot.w, rigidbody->rot.x,
                                              rigidbody->rot.y, rigidbody->rot.z);
    startTransform.setRotation( bt_start_quat ) ;

    // Set starting translation
    startTransform.setOrigin(btVector3(rigidbody->pos.x, rigidbody->pos.y, rigidbody->pos.z));



    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
//    rbInfo.m_restitution = btScalar(0.99f);
//    rbInfo.m_friction = btScalar(0.99f);
    rbInfo.m_restitution = btScalar(0.33f);
    rbInfo.m_friction = btScalar(0.4f);
    btRigidBody* body = new btRigidBody(rbInfo);

    rigidbody->setBody(body);

    { // MUTEX
        PrimT::LockGuard(PhysMutex::dynworld_mx);
        dynamicsWorld->addRigidBody(body);
    }

}

btRigidBody* PhysicsWorld::addStaticTerrainPatch(float* height_data,
                                         unsigned int dim,
                                         float spacing,
                                         float min_height,
                                         float max_height,
                                         glm::vec3 center)
{
    int y_Axis = 1; // y axis is up

    btHeightfieldTerrainShape* heightfieldShape =
        new btHeightfieldTerrainShape(dim, dim,                         // dimentions x, y
                                   height_data,                         // raw data
                                   1.0,                                 // height scale (ignored with float type
                                   // -500.0, 500.0,                       // minheight and maxheight (average must be 0)
                                   min_height, max_height,
                                   y_Axis, PHY_FLOAT, false);           // upaxis = y, type, flipQuadEdges

    // scale the shape
    btVector3 localScaling(spacing, spacing, spacing);
	localScaling[y_Axis] = 1.0;
	heightfieldShape->setLocalScaling(localScaling);

	// stash this shape away
    { // MUTEX
        PrimT::LockGuard guard(PhysMutex::col_shap_mx);
        collisionShapes.push_back(heightfieldShape);
    }

	// set origin to middle of heightfield
	btTransform tr;
	tr.setIdentity();
	//tr.setOrigin(btVector3(0.0, 0.0, 0.0));
	tr.setOrigin(btVector3(center.x, (max_height+min_height)*0.5, center.z));


	// create ground object
	float mass = 0.0;
	//localCreateRigidBody(mass, tr, heightfieldShape);

	btVector3 inertia(0,0,0);
//    if (mass)
//    {
        heightfieldShape->calculateLocalInertia(mass,inertia);
        btRigidBody::btRigidBodyConstructionInfo rbci(mass,0,heightfieldShape,inertia);
        rbci.m_startWorldTransform = tr;
//    }


    btRigidBody* body = new btRigidBody(rbci);
    { // MUTEX:
        PrimT::LockGuard(PhysMutex::dynworld_mx);
        dynamicsWorld->addRigidBody(body);
    }

    std::cout << "finished generating PHYSICAL terrain\n";
    std::cout << "addr of body " << body << "\n";

    return body;
}

void PhysicsWorld::addPhysicsCharContr(DynamicCharacterController* char_contr,
                                       const glm::vec3 &pos,
                                       float height,    // m
                                       float radius,    // m
                                       float mass)      // kg
{
    //char_contr->createCharController(this->dynamicsWorld, this->overlappingPairCache, pos);
    char_contr->setup(this->dynamicsWorld, height, radius, pos, mass);
}

void PhysicsWorld::drawBulletDebug()
{
    glClear(GL_DEPTH_BUFFER_BIT);

    { // MUTEX:
        PrimT::LockGuard(PhysMutex::dynworld_mx);
        dynamicsWorld->debugDrawWorld();
    }
}

void PhysicsWorld::removePhysicsObject(RigidBody* rigidbody)
{
   btRigidBody* body = rigidbody->getBody();

   { // MUTEX:
        PrimT::LockGuard(PhysMutex::dynworld_mx);
        dynamicsWorld->removeRigidBody(body);
   }

   delete body->getMotionState();
   delete body;
}

void PhysicsWorld::removeBtRigidBody(btRigidBody* body)
{
   std::cout << "deleting body " << body << "\n";

   { // MUTEX:
        PrimT::LockGuard(PhysMutex::dynworld_mx);
        dynamicsWorld->removeRigidBody(body);
   }

   delete body->getMotionState();
   delete body;


}

void PhysicsWorld::physicsStep(float dt_in)
{
    { // MUTEX:
        PrimT::LockGuard(PhysMutex::dynworld_mx);
        dynamicsWorld->stepSimulation(dt_in, 3, 1.f/120.f);
    }

//    //print positions of all objects
//    for (int j=dynamicsWorld->getNumCollisionObjects()-1; j>=0 ;j--)
//    {
//        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
//        btRigidBody* body = btRigidBody::upcast(obj);
//        if (body && body->getMotionState())
//        {
//            btTransform trans;
//            body->getMotionState()->getWorldTransform(trans);
//            printf("world pos = %f,%f,%f\n",float(trans.getOrigin().getX()),float(trans.getOrigin().getY()),float(trans.getOrigin().getZ()));
//        }
//    }
}


PhysicsWorld::~PhysicsWorld()
{
    { // MUTEX:
        PrimT::LockGuard(PhysMutex::dynworld_mx);

        for (int i=dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
        {
            btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
            btRigidBody* body = btRigidBody::upcast(obj);
            if (body && body->getMotionState())
            {
                delete body->getMotionState();
            }

            dynamicsWorld->removeCollisionObject( obj );

            delete obj;
        } // FOR
    } // MUTEX

	//delete collision shapes
    { // MUTEX
        PrimT::LockGuard guard(PhysMutex::col_shap_mx);

        for (int j=0;j<collisionShapes.size();j++)
        {
            btCollisionShape* shape = collisionShapes[j];
            collisionShapes[j] = 0;
            delete shape;
        }
    }

	//delete debug draw ?
	delete debugDraw;

	//delete dynamics world
	delete dynamicsWorld;

	//delete solver
	delete solver;

	//delete broadphase
	delete overlappingPairCache;

	//delete dispatcher
	delete dispatcher;

	delete collisionConfiguration;

    // This should not need a mutex...
	collisionShapes.clear();
}
