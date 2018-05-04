#ifndef ECE_6122_FINAL_PROJECT_PHYSICSENGINE_H
#define ECE_6122_FINAL_PROJECT_PHYSICSENGINE_H


#include "objects/Object.h"
#include <iostream>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>


#define DEFAULT_BOUNCINESS  0.1f
#define DEFAULT_FRICTION    0.5f

class PhysicsEngine
{
public:

	PhysicsEngine();
	~PhysicsEngine();

	void stepSimulation(const double &deltaTime);
	void addSphere(float radius, float mass, float bounciness, float friction, glm::vec3 velocity, glm::vec3 position);
	void addBox(glm::vec3 size, float mass, float bounciness, float friction, glm::vec3 velocity, glm::vec3 position);
	void addHull(std::vector<glm::vec3> points, float mass, float bounciness, float friction, glm::vec3 velocity, glm::vec3 position);
	void addWalls(int far);

	void getMotionStates(std::vector<glm::vec3> &states);
	void getOpenGLMatrix(int index, glm::mat4 &matrix);

	static void simple_ball_drop();

private:
	void addObject(
		btCollisionShape *shape,
		btScalar mass,
		btVector3 inertia,
		btDefaultMotionState *motion,
		btScalar bounciness,
		btScalar friction,
		btVector3 velocity
	);

	btBroadphaseInterface *m_broadphase;
	btDefaultCollisionConfiguration *m_colConfig;
	btCollisionDispatcher *m_dispatcher;
	btSequentialImpulseConstraintSolver *m_solver;
	btDiscreteDynamicsWorld *m_world;
	btCollisionShape *m_ground;
	btRigidBody *m_groundRigidBody;
	std::vector<btCollisionShape *> m_shapes;
	std::vector<btRigidBody *> m_rigidBodies;

	btCollisionShape *m_wall1;
	btCollisionShape *m_wall2;
	btCollisionShape *m_wall3;
	btCollisionShape *m_wall4;
	btRigidBody *m_wallRigidBody1;
	btRigidBody *m_wallRigidBody2;
	btRigidBody *m_wallRigidBody3;
	btRigidBody *m_wallRigidBody4;
};


#endif //ECE_6122_FINAL_PROJECT_PHYSICSENGINE_H
