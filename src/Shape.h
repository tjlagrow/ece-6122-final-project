#ifndef ECE_6122_FINAL_PROJECT_RENDERABLE_H
#define ECE_6122_FINAL_PROJECT_RENDERABLE_H

#include "Vertex.h"
#include "Renderer.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class Shape
{
public:
	/**
	 * Constructor
	 * @param vertices TODO
	 * @param indices TODO
	 */
	Shape(glm::mat4 model_transform) : m_model_transform(model_transform)
	{
	}

	/**
	 * Destructor
	 */
	~Shape()
	{
	}

	/**
	 * TODO
	 * @param renderer TODO
	 */
	void submit(Renderer *renderer) const
	{
		renderer->submit(this);
	}

	inline GLuint getNumVertices() const { return m_vertices.size(); }
	inline GLuint getNumIndices() const { return m_indices.size(); }

	inline const std::vector<Vertex> &getVertices() const { return m_vertices; }
	inline const std::vector<GLuint> &getIndices() const { return m_indices; }
	inline const glm::mat4 &getModelTransform() const { return m_model_transform; }

	inline void setVertices(const std::vector<Vertex> &vertices)
	{
		m_vertices = vertices;
	}
	inline void setIndices(const std::vector<GLuint> &indices)
	{
		m_indices = indices;
	}
	inline void setModelTransform(const glm::mat4 model_transform)
	{
		m_model_transform = model_transform;
	}

private:
	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;

	glm::mat4 m_model_transform;

};


#endif //ECE_6122_FINAL_PROJECT_RENDERABLE_H
