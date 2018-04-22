#ifndef ECE_6122_FINAL_PROJECT_MESH_H
#define ECE_6122_FINAL_PROJECT_MESH_H

#include "Position.h"
#include "Normal.h"
#include "Face.h"
#include "TexCoord.h"
#include "Material.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class Mesh
{
public:
	Mesh();
	~Mesh();

	void addFace(Face face);
	void addFace(
		unsigned int px, unsigned int py, unsigned int pz,
		unsigned int tx, unsigned int ty, unsigned int tz,
		unsigned int nx, unsigned int ny, unsigned int nz);

	void setMaterialIndex(GLuint materialIndex);
	void setModelTransform(glm::mat4 modelTransform);

	const std::vector<Face> &getFaces() const;

	const GLuint &getMaterialIndex() const;
	const glm::mat4 &getModelTransform() const;

private:
	std::vector<Face> m_faces;

	GLuint m_materialIndex;
	glm::mat4 m_modelTransform;
};



#endif //ECE_6122_FINAL_PROJECT_MESH_H
