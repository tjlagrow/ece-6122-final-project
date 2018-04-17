#ifndef ECE_6122_FINAL_PROJECT_RIGID_OBJECT_H
#define ECE_6122_FINAL_PROJECT_RIGID_OBJECT_H


#include "../Renderer.h"
#include "Mesh.h"
#include "Material.h"
#include <vector>
#include <unordered_map>
#include <string>

class Renderer;

class RigidObject
{
public:
	RigidObject(const char *filepath);
	~RigidObject();

	void submit(Renderer *renderer) const;

	const std::vector<Mesh> &getMeshes() const;
	const std::unordered_map<unsigned int, Material> &getMaterials() const;

	void setMeshes(std::vector<Mesh> meshes);
	void setMaterials(std::unordered_map<unsigned int, Material> materials);

private:
	RigidObject();

	std::vector<Mesh> m_meshes;
	std::unordered_map<unsigned int, Material> m_materials;
};


#endif //ECE_6122_FINAL_PROJECT_RIGID_OBJECT_H
