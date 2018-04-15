#include "RigidObject.h"

RigidObject::RigidObject() : m_meshes({ })
{

}

RigidObject::~RigidObject()
{

}

void RigidObject::addMesh(Mesh *mesh)
{
	m_meshes.push_back(*mesh);
}

const std::vector<Mesh> &RigidObject::getMeshes() const
{
	return m_meshes;
}

/**
 * This function simply submits this shape to its renderer.
 * It is up to the renderer to actually draw it
 * @param renderer The renderer to use for drawing
 */
void RigidObject::submit(Renderer *renderer) const
{
	renderer->submit(this);
}

