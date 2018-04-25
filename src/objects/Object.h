#ifndef ECE_6122_FINAL_PROJECT_OBJECT_H
#define ECE_6122_FINAL_PROJECT_OBJECT_H


#include "../Renderer.h"
#include "Mesh.h"
#include "Material.h"
#include <vector>
#include <string>

class Renderer;

class Object
{
public:
	Object(const char *filepath);
	~Object();

	void submit(Renderer *renderer) const;

	const std::vector<Mesh> &getMeshes() const;
	const std::vector<Material> &getMaterials() const;
	const std::vector<Position> &getPositions() const;
	std::vector<glm::vec3> getPositionsGlmVec3() const;
	const std::vector<Normal> &getNormals() const;
	const std::vector<TexCoord> &getTexCoords() const;

	const glm::vec3 &getWorldOrigin() const;
	const float &getMass() const;
	const glm::vec3 &getInertia() const;
	const glm::vec3 &getSize() const;

	const Position *getPosition(unsigned int index) const;
	const Normal *getNormal(unsigned int index) const;
	const TexCoord *getTexCoord(unsigned int index) const;

	int getPositionIndex(Position position) const;
	int getNormalIndex(Normal normal) const;
	int getTexCoordIndex(TexCoord texcoord) const;

	void setMass(const float &mass);
	void setInertia(const glm::vec3 &inertia);

	void setTransform(glm::mat4 transform);

	void addMesh(Mesh mesh);
	void addMaterial(Material material);
	unsigned int addPosition(Position position);
	unsigned int addNormal(Normal normal);
	unsigned int addTexCoord(TexCoord texcoord);

private:
	Object();

	std::vector<Mesh> m_meshes;
	std::vector<Material> m_materials;
	std::vector<Position> m_positions;
	std::vector<Normal> m_normals;
	std::vector<TexCoord> m_texcoords;

	glm::vec3 m_worldOrigin;
	glm::vec3 m_origin;

	float m_mass;
	glm::vec3 m_inertia;
	glm::vec3 m_size;
};


#endif //ECE_6122_FINAL_PROJECT_OBJECT_H
