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
	Shape(
		std::vector<Vertex> &vertices,
		std::vector<GLuint> &indices)
		: m_vertices(vertices), m_indices(indices)
	{ }

	/**
	 * Destructor
	 */
	~Shape() { }

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

protected:

	/**
	 * Constructor
	 */
	Shape() { }

	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;

};


#endif //ECE_6122_FINAL_PROJECT_RENDERABLE_H