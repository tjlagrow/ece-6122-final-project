#include "PhysicsEngine.h"

/**
 * TODO Document
 * @param matrix TODO Document
 * @return TODO Document
 */
glm::mat4 btScalar2glmMat4(btScalar* matrix)
{
	return glm::mat4(
		matrix[0], matrix[1], matrix[2], matrix[3],
		matrix[4], matrix[5], matrix[6], matrix[7],
		matrix[8], matrix[9], matrix[10], matrix[11],
		matrix[12], matrix[13], matrix[14], matrix[15]);
}

/**
 * Constructor
 */
PhysicsEngine::PhysicsEngine()
{
	m_broadphase = new btDbvtBroadphase();
	m_colConfig = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_colConfig);
	m_solver = new btSequentialImpulseConstraintSolver();
	m_world = new btDiscreteDynamicsWorld(
		m_dispatcher,
		m_broadphase,
		m_solver,
		m_colConfig
	);

	// The more negative Y is the stronger the gravity. Go ahead, try changing
	// it to (0,-100,0) or something. It's like you're on another planet.
	m_world->setGravity(btVector3(0, -15, 0));

	// For btStaticPlaneShape:
	// The 1st argument is a vector - TODO what does it do?
	// The 2nd argument is a scalar - TODO what does it do?
	m_ground = new btStaticPlaneShape(btVector3(0, 1, 0), 1);

	// This is called with a "new" and normally we'd have to save the variable
	// in order to clean it up later. But we don't do that here because we can
	// get it through the getMotionState() function later
	btDefaultMotionState *groundMotionState = new btDefaultMotionState(
		btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));

	// Create the ground rigid body object and its properties
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(
		0, // Mass is 0 for the ground
		groundMotionState,
		m_ground,
		btVector3(0, 0, 0) // Inertia is 0 for the ground
	);
	m_groundRigidBody = new btRigidBody(groundRigidBodyCI);
	m_groundRigidBody->setFriction(1.0);

	// Finally, add the ground to the world
	m_world->addRigidBody(m_groundRigidBody);
}

/**
 * Destructor
 */
PhysicsEngine::~PhysicsEngine()
{
	for (auto &body : m_rigidBodies)
	{
		m_world->removeRigidBody(body);
		delete body->getMotionState();
		delete body;
	}

	for (auto &shape : m_shapes)
		delete shape;

	m_world->removeRigidBody(m_wallRigidBody1);
	delete m_wallRigidBody1->getMotionState();
	delete m_wallRigidBody1;
	delete m_wall1;
	m_world->removeRigidBody(m_wallRigidBody2);
	delete m_wallRigidBody2->getMotionState();
	delete m_wallRigidBody2;
	delete m_wall2;
	m_world->removeRigidBody(m_wallRigidBody3);
	delete m_wallRigidBody3->getMotionState();
	delete m_wallRigidBody3;
	delete m_wall3;
	m_world->removeRigidBody(m_wallRigidBody4);
	delete m_wallRigidBody4->getMotionState();
	delete m_wallRigidBody4;
	delete m_wall4;

	m_world->removeRigidBody(m_groundRigidBody);
	delete m_groundRigidBody->getMotionState();
	delete m_groundRigidBody;
	delete m_ground;

	delete m_world;
	delete m_solver;
	delete m_dispatcher;
	delete m_colConfig;
	delete m_broadphase;
}

/**
 * Advance the simulation
 * @param deltaTime Change in time from the last time step (usually something
 * like 1/60.0f to 1/20.0f)
 */
void PhysicsEngine::stepSimulation(const double &deltaTime)
{
	m_world->stepSimulation(deltaTime, 10);
}

/**
 * Adds a sphere to the physics engine
 * @param radius The radius of the sphere
 * @param mass The sphere's mass
 * @param bounciness The sphere's bounciness from 0=none to 1=bouncy
 * @param friction The sphere's friction coefficient from 0=low to 1=high
 * https://www.thoughtspike.com/friction-coefficients-for-bullet-physics/
 * @param position The sphere's initial position as measured from the origin
 */
void PhysicsEngine::addSphere(float radius, float mass, float bounciness, float friction, glm::vec3 velocity, glm::vec3 position)
{
	btCollisionShape *shape = new btSphereShape(radius);
	m_shapes.push_back(shape);

	btDefaultMotionState *motion = new btDefaultMotionState(
		btTransform(
			btQuaternion(0, 0, 0, 1),
			btVector3(position.x, position.y, position.z))
	);

	btVector3 inertia(0, 0, 0);
	btVector3 linearVelocity(velocity.x, velocity.y, velocity.z);

	if (bounciness < 0 || bounciness > 1)
		bounciness = DEFAULT_BOUNCINESS;
	if (friction < 0 || friction > 1)
		friction = DEFAULT_FRICTION;

	addObject(shape, mass, inertia, motion, bounciness, friction, linearVelocity);
}

/**
 * Adds a box to the physics engine
 * @param size The size of the box as measured from the origin
 * @param mass The box's mass
 * @param bounciness The sphere's bounciness from 0=none to 1=bouncy
 * @param friction The sphere's friction coefficient from 0=low to 1=high
 * https://www.thoughtspike.com/friction-coefficients-for-bullet-physics/
 * @param position The box's initial position as measured by the origin
 */
void PhysicsEngine::addBox(glm::vec3 size, float mass, float bounciness, float friction, glm::vec3 velocity, glm::vec3 position)
{
	btVector3 boxHalfExtents(size.x, size.y, size.z);

	btCollisionShape *shape = new btBoxShape(boxHalfExtents);
	m_shapes.push_back(shape);

	btDefaultMotionState *motion = new btDefaultMotionState(
		btTransform(
			btQuaternion(0, 0, 0, 1),
			btVector3(position.x, position.y, position.z))
	);

	btVector3 inertia(0, 0, 0);
	btVector3 linearVelocity(velocity.x, velocity.y, velocity.z);

	if (friction < 0 || friction > 1)
		friction = DEFAULT_FRICTION;

	addObject(shape, mass, inertia, motion, bounciness, friction, linearVelocity);
}

/**
 * Add a convex hull shape to the physics engine
 * @param points TODO Document
 * @param mass TODO Document
 * @param bounciness TODO Document
 * @param friction TODO Document
 * @param position TODO Document
 */
void PhysicsEngine::addHull(std::vector<glm::vec3> points, float mass, float bounciness, float friction, glm::vec3 velocity, glm::vec3 position)
{
	btConvexHullShape *shape = new btConvexHullShape();
	for (unsigned int  i = 0; i < points.size(); ++i)
		shape->addPoint(btVector3(points[i].x, points[i].y, points[i].z));

	m_shapes.push_back(shape);

	btDefaultMotionState *motion = new btDefaultMotionState(
		btTransform(
			btQuaternion(0, 0, 0, 1),
			btVector3(position.x, position.y, position.z))
	);

	btVector3 inertia(0, 0, 0);
	btVector3 linearVelocity(velocity.x, velocity.y, velocity.z);

	if (friction < 0 || friction > 1)
		friction = DEFAULT_FRICTION;

	addObject(shape, mass, inertia, motion, bounciness, friction, linearVelocity);
}

/**
 * Add a wall (plane) to the physics engine
 */
void PhysicsEngine::addWalls()
{
//	m_ground = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
//	btDefaultMotionState *groundMotionState = new btDefaultMotionState(
//		btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));

	m_wall1 = new btStaticPlaneShape(btVector3(-1, 0, +0), 1);
	m_wall2 = new btStaticPlaneShape(btVector3(+1, 0, +0), 1);
	m_wall3 = new btStaticPlaneShape(btVector3(+0, 0, -1), 1);
	m_wall4 = new btStaticPlaneShape(btVector3(+0, 0, +1), 1);

	btDefaultMotionState *wallMotionState1= new btDefaultMotionState(
		btTransform(btQuaternion(0, 0, 0, 1), btVector3(11, 0, 0)));
	btDefaultMotionState *wallMotionState2= new btDefaultMotionState(
		btTransform(btQuaternion(0, 0, 0, 1), btVector3(-11, 0, 0)));
	btDefaultMotionState *wallMotionState3= new btDefaultMotionState(
		btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 8)));
	btDefaultMotionState *wallMotionState4= new btDefaultMotionState(
		btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, -8)));

	btRigidBody::btRigidBodyConstructionInfo wallRigidBodyCI1(
		0, // Walls have 0 mass
		wallMotionState1,
		m_wall1,
		btVector3(0, 0, 0) // Walls havae 0 inertia
	);
	btRigidBody::btRigidBodyConstructionInfo wallRigidBodyCI2(
		0, // Walls have 0 mass
		wallMotionState2,
		m_wall2,
		btVector3(0, 0, 0) // Walls havae 0 inertia
	);
	btRigidBody::btRigidBodyConstructionInfo wallRigidBodyCI3(
		0, // Walls have 0 mass
		wallMotionState3,
		m_wall3,
		btVector3(0, 0, 0) // Walls havae 0 inertia
	);
	btRigidBody::btRigidBodyConstructionInfo wallRigidBodyCI4(
		0, // Walls have 0 mass
		wallMotionState4,
		m_wall4,
		btVector3(0, 0, 0) // Walls havae 0 inertia
	);

	m_wallRigidBody1 = new btRigidBody(wallRigidBodyCI1);
	m_wallRigidBody1->setFriction(0.5);
	m_wallRigidBody1->setRestitution(0.5);
	m_wallRigidBody2 = new btRigidBody(wallRigidBodyCI2);
	m_wallRigidBody2->setFriction(0.5);
	m_wallRigidBody2->setRestitution(0.5);
	m_wallRigidBody3 = new btRigidBody(wallRigidBodyCI3);
	m_wallRigidBody3->setFriction(0.5);
	m_wallRigidBody3->setRestitution(0.5);
	m_wallRigidBody4 = new btRigidBody(wallRigidBodyCI4);
	m_wallRigidBody4->setFriction(0.5);
	m_wallRigidBody4->setRestitution(0.5);

	m_world->addRigidBody(m_wallRigidBody1);
	m_world->addRigidBody(m_wallRigidBody2);
	m_world->addRigidBody(m_wallRigidBody3);
	m_world->addRigidBody(m_wallRigidBody4);
}

/**
 * TODO Document
 * @param states TODO Document
 */
void PhysicsEngine::getMotionStates(std::vector<glm::vec3> &states)
{
	if (m_rigidBodies.size() > 0)
		states.clear();

	for (unsigned int i = 0; i < m_rigidBodies.size(); ++i)
	{
		btTransform worldTransform;
		m_rigidBodies[i]->getMotionState()->getWorldTransform(worldTransform);
		glm::vec3 position(
			worldTransform.getOrigin().x(),
			worldTransform.getOrigin().y(),
			worldTransform.getOrigin().z()
		);
		states.push_back(position);
	}
}

/**
 * TODO Document
 * @param index TODO Document
 * @param matrix TODO Document
 */
void PhysicsEngine::getOpenGLMatrix(int index, glm::mat4 &matrix)
{
	if (m_rigidBodies.empty())
		return;

	btScalar trans[16];
	btTransform worldTransform;

	m_rigidBodies[index]->getMotionState()->getWorldTransform(worldTransform);
	worldTransform.getOpenGLMatrix(trans);
	matrix = btScalar2glmMat4(trans);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

/**
 * TODO Document
 * @param shape TODO Document
 * @param mass TODO Document
 * @param inertia TODO Document
 * @param motion TODO Document
 */
void PhysicsEngine::addObject(
	btCollisionShape *shape,
	btScalar mass,
	btVector3 inertia,
	btDefaultMotionState *motion,
	btScalar bounciness,
	btScalar friction,
	btVector3 velocity)
{
	shape->calculateLocalInertia(mass, inertia);

	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
		mass,
		motion,
		shape,
		inertia);

	auto *rigidBody = new btRigidBody(rigidBodyCI);
	rigidBody->setRestitution(bounciness);
	rigidBody->setFriction(friction);
	rigidBody->setLinearVelocity(velocity);

	m_rigidBodies.push_back(rigidBody);
	m_world->addRigidBody(rigidBody);
}

