#include "Layer.h"

/**
 * Constructor
 * @param shader TODO
 */
Layer::Layer(Shader *shader)
	: Layer(new Renderer(), shader)
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
void Layer::add(RigidObject *object)
{
	m_objects.push_back(object);
	m_renderer->addMeshes(object->getMeshes().size());
}

/**
 * Renders the layer
 * TODO Add more description
 */
void Layer::render()
{
	m_shader->enable();

	for (const RigidObject *object : m_objects)
	{
		object->submit(m_renderer);
	}

	m_renderer->flush();
	m_shader->disable();
}
