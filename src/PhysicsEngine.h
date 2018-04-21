#ifndef ECE_6122_FINAL_PROJECT_PHYSICSENGINE_H
#define ECE_6122_FINAL_PROJECT_PHYSICSENGINE_H


#include "meshes/RigidObject.h"
#include <iostream>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

class PhysicsEngine
{
public:

	PhysicsEngine();
	~PhysicsEngine();

	void stepSimulation(const double &deltaTime);
	void addObject(const RigidObject *object);
	void addObjects(const std::vector<RigidObject *> &objects);
	void addSphere();
	void addBox(
		const glm::vec3 &boundaries,
		const glm::vec3 &position);

	void getMotionStates(std::vector<glm::vec3> &states);
	void getOpenGLMatrix(int index, glm::mat4 &matrix);

	static void simple_ball_drop();

private:
	btBroadphaseInterface *m_broadphase;
	btDefaultCollisionConfiguration *m_colConfig;
	btCollisionDispatcher *m_dispatcher;
	btSequentialImpulseConstraintSolver *m_solver;
	btDiscreteDynamicsWorld *m_world;
	btCollisionShape *m_ground;
	btRigidBody *m_groundRigidBody;
	std::vector<btCollisionShape *> m_shapes;
	std::vector<btRigidBody *> m_rigidBodies;
};


#endif //ECE_6122_FINAL_PROJECT_PHYSICSENGINE_H
