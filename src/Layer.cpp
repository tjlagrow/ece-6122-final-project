#include <iostream>
#include "Layer.h"
#include "BatchRenderer.h"

/**
 * Constructor
 * @param shader TODO
 */
Layer::Layer(Shader *shader)
	: Layer(new BatchRenderer(), shader, glm::mat4())
{
}

/**
 * Constructor
 * @param renderer TODO
 * @param shader TODO
 * @param pmat TODO
 */
Layer::Layer(Renderer *renderer, Shader *shader, const glm::mat4 &pmat)
	: m_renderer(renderer), m_shader(shader), m_pmat(pmat)
{
	m_shader->enable();
	m_shader->setUniformMat4("vpmat", m_pmat);
	m_shader->disable();
}

/**
 * Destructor
 */
Layer::~Layer()
{
	std::cout << __func__ << std::endl;
	delete m_renderer;
//	for (unsigned int i = 0; i < m_objects.size(); ++i)
//		delete m_objects[i];
}

/**
 * Adds (pushes back) a renderable object to the layer
 * @param renderable The object to add
 */
void Layer::add(Shape *renderable)
{
	m_shapes.push_back(renderable);
}

/**
 * Renders the layer.
 * TODO Add more description
 */
void Layer::render()
{
	m_shader->enable();
	m_renderer->begin();

	for (const Shape *shape : m_shapes)
		shape->submit(m_renderer);

	m_renderer->end();
	m_shader->disable();
}
