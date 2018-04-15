#include "Mesh.h"

/**
 * Constructor
 * @param model_transform TODO
 */
Mesh::Mesh() :
	m_vertices(),
	m_faces(),
	m_model_transform(glm::mat4(1.0f))
{
}

/**
 * Constructor
 * @param model_transform TODO
 */
Mesh::Mesh(glm::mat4 model_transform) :
	m_vertices(),
	m_faces(),
	m_model_transform(model_transform)
{
}

/**
 * Constructor
 * @param model_transform TODO
 */
Mesh::Mesh(
	std::vector<Vertex> vertices,
	std::vector<Face> faces,
	glm::mat4 model_transform) :
	m_vertices(vertices),
	m_faces(faces),
	m_model_transform(model_transform)
{
}

/**
 * Destructor
 */
Mesh::~Mesh()
{
}

/**
 * This function simply submits this shape to its renderer.
 * It is up to the renderer to actually draw it
 * @param renderer The renderer to use for drawing
 */
void Mesh::submit(Renderer *renderer) const
{
	renderer->submit(this);
}

/**
 * Get the vertices vector
 * @return A vector of vertices for this shape
 */
std::vector<Vertex> Mesh::getVertices() const
{
	return m_vertices;
}

/**
 * Get the indices vector
 * @return A vector of indices for this shape
 */
const std::vector<Face> &Mesh::getFaces() const
{
	return m_faces;
}

/**
 * Get the model transform matrix for the shape
 * @return The model transform matrix for this shape
 */
const glm::mat4 &Mesh::getModelTransform() const
{
	return m_model_transform;
}

/**
 * Set the vertices for this shape
 * @param vertices The new vector of vertices
 */
//void Mesh::setVertices(const std::vector<Vertex> &vertices)
//{
//	m_vertices = vertices;
//}

/**
 * Set the indices for this shape
 * @param indices The new vector of indices
 */
//void Mesh::setIndices(const std::vector<GLuint> &indices)
//{
//	m_indices = indices;
//}

