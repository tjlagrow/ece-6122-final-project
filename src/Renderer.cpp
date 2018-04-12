#include "Renderer.h"

/**
 * Constructor
 */
Renderer::Renderer()
{
	m_transformations.push_back(glm::mat4());
	m_back_transform = &m_transformations.back();
}

/**
 * Destructor
 */
Renderer::~Renderer() { }

/**
 * Begin the render with common initialization code here
 */
void Renderer::begin() { }

/**
 * End the render with common cleanup code here
 */
void Renderer::end() { }

/**
 * Pushes a transformation matrix
 * @param matrix TODO
 * @param override TODO
 */
void Renderer::push(glm::mat4 matrix, bool override)
{
	if (override)
		m_transformations.push_back(matrix);
	else
		m_transformations.push_back(m_transformations.back() * matrix);

	m_back_transform = &m_transformations.back();
}

/**
 * Pops a transformation matrix
 */
void Renderer::pop()
{
	if (m_transformations.size() > 1)
		m_transformations.pop_back();

	m_back_transform = &m_transformations.back();
}
