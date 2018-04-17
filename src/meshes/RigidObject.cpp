#include "RigidObject.h"
#include "ObjectLoader.h"

/**
 * Constructor
 */
RigidObject::RigidObject()
	: m_meshes({ }), m_materials({ })
{
}

/**
 * Constructor
 * @param filepath TODO
 */
RigidObject::RigidObject(const char *filepath)
	: m_meshes({ }), m_materials({ })
{
	ObjectLoader::loadFromFile(filepath, this);
}

/**
 * Destructor
 */
RigidObject::~RigidObject()
{

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

/**
 * Get the meshes that make up this object
 * @return Returns this object's meshes
 */
const std::vector<Mesh> &RigidObject::getMeshes() const
{
	return m_meshes;
}

/**
 * Sets this object's meshes
 * @param meshes The meshes to be set
 */
void RigidObject::setMeshes(std::vector<Mesh> meshes)
{
	m_meshes = meshes;
}

/**
 * Sets this object's materials
 * @param materials The materials to be set
 */
void RigidObject::setMaterials(std::unordered_map<unsigned int, Material> materials)
{
	m_materials = materials;
}

/**
 * TODO
 * @param transform TODO
 */
void RigidObject::applyTransform(glm::mat4 transform)
{
	for (auto &mesh : m_meshes)
		mesh.setModelTransform(transform);
}
