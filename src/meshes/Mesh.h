#ifndef ECE_6122_FINAL_PROJECT_MESH_H
#define ECE_6122_FINAL_PROJECT_MESH_H

#include "Position.h"
#include "Normal.h"
#include "Face.h"
#include "TexCoord.h"
#include "Color.h"
#include "Material.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

class Mesh
{
public:
	Mesh();
	~Mesh();

	void addPosition(Position position);
	void addNormal(Normal normal);
	void addFace(Face face);
	void addTexCoord(TexCoord texcoord);
	void addColor(Color color);

	void setMaterialIndex(GLuint materialIndex);
	void setModelTransform(glm::mat4 modelTransform);

	const std::vector<Position> &getPositions() const;
	const std::vector<Normal> &getNormals() const;
	const std::vector<Face> &getFaces() const;
	const std::vector<TexCoord> &getTexCoords() const;
	const std::vector<Color> &getColors() const;

	const GLuint &getMaterialIndex() const;
	const glm::mat4 &getModelTransform() const;

protected:
	std::vector<Position> m_positions;
	std::vector<Normal> m_normals;
	std::vector<Face> m_faces;
	std::vector<TexCoord> m_texcoords;
	std::vector<Color> m_colors;

	GLuint m_materialIndex;
	glm::mat4 m_modelTransform;

private:

};



#endif //ECE_6122_FINAL_PROJECT_MESH_H
