#include "ArgParser.h"
#include <iostream>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>


int main(int argc, char **argv)
{
	ProgramConfig cfg;
	ArgParser::parse_args(argc, argv, cfg);

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
	btCollisionShape *sphereShape = new btSphereShape(1);

	btDefaultMotionState *groundMotionState =new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	btRigidBody *groundRigidBody = new btRigidBody(groundRigidBodyCI);
	world->addRigidBody(groundRigidBody);

	btDefaultMotionState *sphereMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
	btScalar sphereMass = 1;
	btVector3 sphereInertia(0, 0, 0);
	sphereShape->calculateLocalInertia(sphereMass, sphereInertia);
	btRigidBody::btRigidBodyConstructionInfo sphereRigidBodyCI(sphereMass, sphereMotionState, sphereShape, sphereInertia);
	btRigidBody *sphereRigidBody = new btRigidBody(sphereRigidBodyCI);
	world->addRigidBody(sphereRigidBody);

	for (int i = 0; i < 300; i++)
	{
		world->stepSimulation(1/60.0f, 10);
		btTransform trans;
		sphereRigidBody->getMotionState()->getWorldTransform(trans);
		printf("%f\n", trans.getOrigin().getY());
	}

	world->removeRigidBody(sphereRigidBody);
	delete sphereRigidBody->getMotionState();
	delete sphereRigidBody;

	world->removeRigidBody(groundRigidBody);
	delete groundRigidBody->getMotionState();
	delete groundRigidBody;

	delete sphereShape;
	delete groundShape;

	// Clean up all the bullet memory
	delete world;
	delete solver;
	delete dispatcher;
	delete colConfig;
	delete broadphase;

	return (0);
}
