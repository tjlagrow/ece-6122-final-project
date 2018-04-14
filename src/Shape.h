#ifndef ECE_6122_FINAL_PROJECT_RENDERABLE_H
#define ECE_6122_FINAL_PROJECT_RENDERABLE_H

#include "Vertex.h"
#include "BatchRenderer.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class BatchRenderer;

class Shape
{
public:
	Shape(glm::mat4 model_transform);
	~Shape();

	void submit(BatchRenderer *renderer) const;

	std::vector<Vertex> getVertices() const;
	const std::vector<GLuint> &getIndices() const;
	const glm::mat4 &getModelTransform() const;

	void setModelTransform(const glm::mat4 model_transform);

protected:
	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;

	glm::mat4 m_model_transform;

private:

};



#endif //ECE_6122_FINAL_PROJECT_RENDERABLE_H
