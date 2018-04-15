#ifndef ECE_6122_FINAL_PROJECT_OBJECT_H
#define ECE_6122_FINAL_PROJECT_OBJECT_H

#include "../Renderer.h"
#include "Mesh.h"
#include <vector>

class Renderer;

class RigidObject
{
public:
	RigidObject();
	~RigidObject();

	void addMesh(Mesh *mesh);
	void submit(Renderer *renderer) const;

	const std::vector<Mesh> &getMeshes() const;

private:
	std::vector<Mesh> m_meshes;
};


#endif //ECE_6122_FINAL_PROJECT_OBJECT_H
