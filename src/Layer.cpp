#include "Layer.h"

/**
 * Constructor
 * @param shader TODO
 */
Layer::Layer(Shader *shader)
	: Layer(new Renderer(shader), shader)
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
}

/**
 * Destructor
 */
Layer::~Layer()
{
	delete m_renderer;
}

/**
 * Adds (pushes back) a shape to the layer
 * @param shape The object to add
 */
void Layer::add(Object *object)
{
	m_objects.push_back(object);
}

/**
 * Renders the layer by submitting the objects to the Renderer, then
 * flushing the Renderer buffer (aka draw the triangles)
 */
void Layer::render(const glm::vec3 &eyePosition)
{
	m_shader->enable();

	for (const Object *object : m_objects)
	{
		object->submit(m_renderer);
	}

	m_renderer->flush(eyePosition);
	m_shader->disable();
}

/**
 * Returns the object by index
 * @param index The index into the object array
 * @return Returns the pointer to the object on success, else NULL
 */
Object *Layer::getObject(int index)
{
	if (index < 0 || index >= (int)m_objects.size())
		return nullptr;
	return m_objects[index];
}

/**
 * Get all the objects in this layer
 * @return Returns all the objects in this layer
 */
const std::vector<Object *> &Layer::getObjects()
{
	return m_objects;
}
