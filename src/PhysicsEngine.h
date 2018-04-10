#ifndef ECE_6122_FINAL_PROJECT_PHYSICSENGINE_H
#define ECE_6122_FINAL_PROJECT_PHYSICSENGINE_H

#include <iostream>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

class PhysicsEngine
{
public:

	PhysicsEngine();
	~PhysicsEngine();

	void stepSimulation(const double &deltaTime);

	static void simple_ball_drop();

private:
	btBroadphaseInterface *m_broadphase;
	btDefaultCollisionConfiguration *m_colConfig;
	btCollisionDispatcher *m_dispatcher;
	btSequentialImpulseConstraintSolver *m_solver;
	btDiscreteDynamicsWorld *m_world;
	btCollisionShape *m_ground;
	btCollisionShape *m_sphere;
	btRigidBody *m_groundRigidBody;
	btRigidBody *m_sphereRigidBody;
};


#endif //ECE_6122_FINAL_PROJECT_PHYSICSENGINE_H
