#include "Renderer.h"


Renderer::Renderer()
{
	m_transformations.push_back(glm::mat4());
	m_back_transform = &m_transformations.back();
}

Renderer::~Renderer() { }

void Renderer::begin()
{
}

void Renderer::end()
{
}

void Renderer::push(glm::mat4 matrix, bool override)
{
	if (override)
		m_transformations.push_back(matrix);
	else
		m_transformations.push_back(m_transformations.back() * matrix);

	m_back_transform = &m_transformations.back();
}

void Renderer::pop()
{
	m_transformations.push_back(glm::mat4());
	m_back_transform = &m_transformations.back();
}
