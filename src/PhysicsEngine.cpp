#include "PhysicsEngine.h"

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

	m_world->setGravity(btVector3(0, -10, 0));

	m_ground = new btStaticPlaneShape(btVector3(0, 1, 0), 1);

	btDefaultMotionState *groundMotionState = new btDefaultMotionState(
		btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));

	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(
		0,
		groundMotionState,
		m_ground,
		btVector3(0, 0, 0)
	);

	m_groundRigidBody = new btRigidBody(groundRigidBodyCI);

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
 * @param deltaTime Change in time from the last time step (usually 1/60.0f)
 */
void PhysicsEngine::stepSimulation(const double &deltaTime)
{
	m_world->stepSimulation(deltaTime, 10);
}

void PhysicsEngine::addObject(const RigidObject *object)
{
}

void PhysicsEngine::addObjects(const std::vector<RigidObject *> &objects)
{
	for (unsigned int i = 0; i < objects.size(); ++i)
		addObject(objects[i]);
}

void PhysicsEngine::addSphere()
{
	btCollisionShape *sphereShape = new btSphereShape(1);
	btDefaultMotionState *sphereMotionState = new btDefaultMotionState(
		btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
	btScalar sphereMass = 1;
	btVector3 sphereInertia(0, 0, 0);
	sphereShape->calculateLocalInertia(sphereMass, sphereInertia);
	btRigidBody::btRigidBodyConstructionInfo sphereRigidBodyCI(
		sphereMass,
		sphereMotionState,
		sphereShape,
		sphereInertia);
	btRigidBody *sphereRigidBody = new btRigidBody(sphereRigidBodyCI);
	m_world->addRigidBody(sphereRigidBody);
}

/**
 * TODO
 * @param boundaries TODO
 * @param position TODO
 */
void PhysicsEngine::addBox(
	const glm::vec3 &boundaries,
	const glm::vec3 &position)
{
	btVector3 boxHalfExtents(boundaries.x, boundaries.y, boundaries.z);

	btCollisionShape *shape = new btBoxShape(boxHalfExtents);
	m_shapes.push_back(shape);

	btDefaultMotionState *motionState = new btDefaultMotionState(
		btTransform(
			btQuaternion(0, 0, 0, 1),
			btVector3(position.x, position.y, position.z))
	);

	btScalar mass = 1;
	btVector3 inertia(0, 0, 0);
	shape->calculateLocalInertia(mass, inertia);

	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
		mass,        // btScalar
		motionState, // pointer to btDefaultMotionState
		shape,       // pointer to btCollisionShape
		inertia);    // btVector3

	btRigidBody *rigidBody = new btRigidBody(rigidBodyCI);

	m_rigidBodies.push_back(rigidBody);
	m_world->addRigidBody(rigidBody);
}

glm::mat4 btScalar2glmMat4(btScalar* matrix) {
	return glm::mat4(
		matrix[0], matrix[1], matrix[2], matrix[3],
		matrix[4], matrix[5], matrix[6], matrix[7],
		matrix[8], matrix[9], matrix[10], matrix[11],
		matrix[12], matrix[13], matrix[14], matrix[15]);
}

void PhysicsEngine::getMotionStates(
	std::vector<glm::vec3> &states)
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
