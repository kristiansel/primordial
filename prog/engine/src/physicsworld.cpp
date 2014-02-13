#include "physicsworld.h"

PhysicsWorld::PhysicsWorld()
{
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new	btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;

    /// Initialize the world
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,solver,collisionConfiguration);

    /// Set gravity
	dynamicsWorld->setGravity(btVector3(0,-9.81,0));

//	/// ADD THE GROUND (LATER DELEGATE THIS TO SOMETHING ELSE)
//    ///create a few basic rigid bodies
//	btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.),btScalar(50.),btScalar(50.)));
//
//	//keep track of the shapes, we release memory at exit.
//	//make sure to re-use collision shapes among rigid bodies whenever possible!
//
//	collisionShapes.push_back(groundShape);
//
//	btTransform groundTransform;
//	groundTransform.setIdentity();
//	groundTransform.setOrigin(btVector3(0,-56,0)); /// Translate the box
//
//	/// Under construction
//
//	{
//		btScalar mass(0.);
//
//		//rigidbody is dynamic if and only if mass is non zero, otherwise static
//		bool isDynamic = (mass != 0.f);
//
//		btVector3 localInertia(0,0,0);
//		if (isDynamic)
//			groundShape->calculateLocalInertia(mass,localInertia);
//
//		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
//		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
//		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape,localInertia);
//		btRigidBody* body = new btRigidBody(rbInfo);
//
//		//add the body to the dynamics world
//		dynamicsWorld->addRigidBody(body);
//	}
}

// void PhysicsWorld::addPhysicsObject(RigidBody::Collision shape, par1=0, par2=0, par3=0, par4=0)
void PhysicsWorld::addPhysicsDynamic(RigidBody* rigidbody)
{
    //create a dynamic rigidbody

    //btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
    btCollisionShape* colShape = new btSphereShape(btScalar(1.));
    collisionShapes.push_back(colShape);

    /// Create Dynamic Objects
    btTransform startTransform;
    startTransform.setIdentity();

    btScalar	mass(10.f);

    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0,0,0);
    if (isDynamic)
        colShape->calculateLocalInertia(mass,localInertia);

//    /// All these lines, just to convert quaternions
//    Quat start_quat = Quat(rigidbody->dir, glm::vec3(0.0, 0.0, -1.0));
//    btQuaternion bt_start_quat = btQuaternion(btScalar(start_quat.w),
//                                              btScalar(start_quat.x),
//                                              btScalar(start_quat.y),
//                                              btScalar(start_quat.z));

    btQuaternion bt_start_quat = btQuaternion(rigidbody->rot.w, rigidbody->rot.x,
                                              rigidbody->rot.y, rigidbody->rot.z);

    //std::cout << "sq(" << start_quat.w << ", " << start_quat.x << ", " << start_quat.y << ", " << start_quat.z << ")\n";

    /// Currently there is a bug where rotations cause adding and deleting from scene to break
    startTransform.setRotation( bt_start_quat ) ;
    startTransform.setOrigin(btVector3(rigidbody->pos.x, rigidbody->pos.y, rigidbody->pos.z));


    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
    rbInfo.m_restitution = btScalar(0.6f);
    rbInfo.m_friction = btScalar(0.4f);
    btRigidBody* body = new btRigidBody(rbInfo);

    rigidbody->setBody(body);

    dynamicsWorld->addRigidBody(body);
}

void PhysicsWorld::addPhysicsStatic(RigidBody* rigidbody)
{
    //create a dynamic rigidbody

    //btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
    /// I think this puts the plane at -1 y?
    btCollisionShape* colShape = new btStaticPlaneShape(btVector3(0.0, 1.0, 0.0), 0.0);
    collisionShapes.push_back(colShape);

    /// Create Dynamic Objects
    btTransform startTransform;
    startTransform.setIdentity();

    btScalar	mass(0.f);

    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);

    btVector3 localInertia(0,0,0);
    if (isDynamic)
        colShape->calculateLocalInertia(mass,localInertia);

    /// Do not think transform is necessary for infinite plane?
    // startTransform.setOrigin(btVector3(rigidbody->pos.x, rigidbody->pos.y, rigidbody->pos.z));

    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
    rbInfo.m_restitution = btScalar(0.99f);
    rbInfo.m_friction = btScalar(0.99f);
    btRigidBody* body = new btRigidBody(rbInfo);

    rigidbody->setBody(body);

    dynamicsWorld->addRigidBody(body);
}

void PhysicsWorld::removePhysicsObject(RigidBody* rigidbody)
{
   btRigidBody* body = rigidbody->getBody();
   dynamicsWorld->removeRigidBody(body);
   delete body->getMotionState();
   delete body;
}

void PhysicsWorld::physicsStep(float dt_in)
{
    dynamicsWorld->stepSimulation(dt_in, 3, 1.f/120.f);

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
	}

	//delete collision shapes
	for (int j=0;j<collisionShapes.size();j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}
	//delete dynamics world
	delete dynamicsWorld;

	//delete solver
	delete solver;

	//delete broadphase
	delete overlappingPairCache;

	//delete dispatcher
	delete dispatcher;

	delete collisionConfiguration;

	collisionShapes.clear();
}
