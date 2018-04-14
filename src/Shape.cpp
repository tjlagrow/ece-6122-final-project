#include "Shape.h"

/**
 * Constructor
 * @param model_transform TODO
 */
Shape::Shape(glm::mat4 model_transform) :
	m_model_transform(model_transform)
{
}

/**
 * Destructor
 */
Shape::~Shape()
{
}

/**
 * This function simply submits this shape to its renderer.
 * It is up to the renderer to actually draw it
 * @param renderer The renderer to use for drawing
 */
void Shape::submit(Renderer *renderer) const
{
	renderer->submit(this);
}

/**
 * Get the vertices vector
 * @return A vector of vertices for this shape
 */
std::vector<Vertex> Shape::getVertices() const
{
	return m_vertices;
}

/**
 * Get the indices vector
 * @return A vector of indices for this shape
 */
const std::vector<GLuint> &Shape::getIndices() const
{
	return m_indices;
}

/**
 * Get the model transform matrix for the shape
 * @return The model transform matrix for this shape
 */
const glm::mat4 &Shape::getModelTransform() const
{
	return m_model_transform;
}

/**
 * Set the vertices for this shape
 * @param vertices The new vector of vertices
 */
//void Shape::setVertices(const std::vector<Vertex> &vertices)
//{
//	m_vertices = vertices;
//}

/**
 * Set the indices for this shape
 * @param indices The new vector of indices
 */
//void Shape::setIndices(const std::vector<GLuint> &indices)
//{
//	m_indices = indices;
//}

/**
 * Set the model transform matrix for this shape
 * @param model_transform The shape's model transform matrix
 */
void Shape::setModelTransform(const glm::mat4 model_transform)
{
	m_model_transform = model_transform;
}
