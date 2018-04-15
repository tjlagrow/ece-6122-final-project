#ifndef ECE_6122_FINAL_PROJECT_MESH_H
#define ECE_6122_FINAL_PROJECT_MESH_H

#include "../Vertex.h"
#include "../Face.h"
#include "../Renderer.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class Renderer;

class Mesh
{
public:
	Mesh();
	Mesh(glm::mat4 model_transform);
	Mesh(std::vector<Vertex> vertices, std::vector<Face> faces, glm::mat4 model_transform);
	~Mesh();

	void submit(Renderer *renderer) const;

	std::vector<Vertex> getVertices() const;
	const std::vector<Face> &getFaces() const;
	const glm::mat4 &getModelTransform() const;

protected:
	std::vector<Vertex> m_vertices;
	std::vector<Face> m_faces;
	glm::mat4 m_model_transform;

private:

};



#endif //ECE_6122_FINAL_PROJECT_MESH_H
