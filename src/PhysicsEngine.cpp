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
 * Demo getting started function
 */
void PhysicsEngine::simple_ball_drop()
{
	// Choose the broadphase algorithm
	// http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Broadphase
	btBroadphaseInterface *broadphase = new btDbvtBroadphase();

	// The collision configuration allows you to fine tune the algorithms used for the full (not broadphase)
	// collision detection. http://www.bulletphysics.org/mediawiki-1.5.8/index.php/Collision_Things
	btDefaultCollisionConfiguration *colConfig = new btDefaultCollisionConfiguration();
	btCollisionDispatcher *dispatcher = new btCollisionDispatcher(colConfig);

	// We also need a "solver". This is what causes the objects to interact properly, taking into account gravity,
	// game logic supplied forces, collisions, and hinge constraints. It does a good job as long as you don't push
	// it to extremes, and is one of the bottlenecks in any high performance simulation. There are parallel versions
	// available for some threading models.
	btSequentialImpulseConstraintSolver *solver = new btSequentialImpulseConstraintSolver();

	// Instantiate the dynamics world
	btDiscreteDynamicsWorld *world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, colConfig);

	// Choose Y-axis to be up
	world->setGravity(btVector3(0, -10, 0));

	btCollisionShape *groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	btDefaultMotionState *groundMotionState = new btDefaultMotionState(
		btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	btRigidBody *groundRigidBody = new btRigidBody(groundRigidBodyCI);
	world->addRigidBody(groundRigidBody);

	btCollisionShape *sphereShape = new btSphereShape(1);
	btDefaultMotionState *sphereMotionState = new btDefaultMotionState(
		btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
	btScalar sphereMass = 1;
	btVector3 sphereInertia(0, 0, 0);
	sphereShape->calculateLocalInertia(sphereMass, sphereInertia);
	btRigidBody::btRigidBodyConstructionInfo sphereRigidBodyCI(sphereMass, sphereMotionState, sphereShape,
															   sphereInertia);
	btRigidBody *sphereRigidBody = new btRigidBody(sphereRigidBodyCI);
	world->addRigidBody(sphereRigidBody);

	for (int i = 0; i < 300; i++)
	{
		world->stepSimulation(1 / 60.0f, 10);
		btTransform trans;
		sphereRigidBody->getMotionState()->getWorldTransform(trans);
		printf("%f\n", trans.getOrigin().getY());
	}

	// Clean up all the bullet memory

	world->removeRigidBody(sphereRigidBody);
	delete sphereRigidBody->getMotionState();
	delete sphereRigidBody;

	world->removeRigidBody(groundRigidBody);
	delete groundRigidBody->getMotionState();
	delete groundRigidBody;

	delete sphereShape;
	delete groundShape;

	delete world;
	delete solver;
	delete dispatcher;
	delete colConfig;
	delete broadphase;
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
		btVector3(0, 0, 0) // Intertia is 0 for the ground
	);
	m_groundRigidBody = new btRigidBody(groundRigidBodyCI);

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
void PhysicsEngine::addSphere(float radius, float mass, float bounciness, float friction, glm::vec3 position)
{
	btCollisionShape *shape = new btSphereShape(radius);
	m_shapes.push_back(shape);

	btDefaultMotionState *motion = new btDefaultMotionState(
		btTransform(
			btQuaternion(0, 0, 0, 1),
			btVector3(position.x, position.y, position.z))
	);

	btVector3 inertia(0, 0, 0);

	if (bounciness < 0 || bounciness > 1)
		bounciness = DEFAULT_BOUNCINESS;
	if (friction < 0 || friction > 1)
		friction = DEFAULT_FRICTION;

	addObject(shape, mass, inertia, motion, bounciness, friction);
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
void PhysicsEngine::addBox(glm::vec3 size, float mass, float bounciness, float friction, glm::vec3 position)
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

	if (friction < 0 || friction > 1)
		friction = DEFAULT_FRICTION;

	addObject(shape, mass, inertia, motion, bounciness, friction);
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
	btScalar friction)
{
	shape->calculateLocalInertia(mass, inertia);

	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
		mass,
		motion,
		shape,
		inertia);

	auto *rigidBody = new btRigidBody(rigidBodyCI);
	rigidBody->setRestitution(bounciness);
	rigidBody->setFriction(1.0);

	m_rigidBodies.push_back(rigidBody);
	m_world->addRigidBody(rigidBody);
}

