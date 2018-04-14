#include "Layer.h"
#include "BatchRenderer.h"
#include <iostream>

/**
 * Constructor
 * @param shader TODO
 */
Layer::Layer(Shader *shader)
	: Layer(new BatchRenderer(), shader)
{
}

/**
 * Constructor
 * @param renderer TODO
 * @param shader TODO
 * @param pmat TODO
 */
Layer::Layer(Renderer *renderer, Shader *shader)
	: m_renderer(renderer), m_shader(shader)
{
//	m_shader->enable();
//	m_shader->setUniformMat4("vpmat", m_pmat);
//	m_shader->disable();
}

/**
 * Destructor
 */
Layer::~Layer()
{
	delete m_renderer;
//	for (unsigned int i = 0; i < m_objects.size(); ++i)
//		delete m_objects[i];
}

/**
 * Adds (pushes back) a shape to the layer
 * @param shape The object to add
 */
void Layer::add(Shape *shape)
{
	m_shapes.push_back(shape);
}

/**
 * Renders the layer
 * TODO Add more description
 */
void Layer::render()
{
	m_shader->enable();
	m_renderer->begin();

	for (const Shape *shape : m_shapes)
	{
		m_shader->setUniformMat4("vmmat", shape->getModelTransform());
		shape->submit(m_renderer);
	}

	m_renderer->end();
	m_renderer->flush();
	m_shader->disable();
}
