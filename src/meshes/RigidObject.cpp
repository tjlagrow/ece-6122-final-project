#include "RigidObject.h"
#include "ObjectLoader.h"

/**
 * Constructor
 */
RigidObject::RigidObject()
{
}

/**
 * Constructor
 * @param filepath TODO
 */
RigidObject::RigidObject(const char *filepath)
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
 * TODO
 * @return TODO
 */
const std::vector<Material> &RigidObject::getMaterials() const
{
	return m_materials;
}

/**
 * TODO
 * @return TODO
 */
const std::vector<Position> &RigidObject::getPositions() const
{
	return m_positions;
}

/**
 * TODO
 * @return TODO
 */
const std::vector<Normal> &RigidObject::getNormals() const
{
	return m_normals;
}

/**
 * TODO
 * @return TODO
 */
const std::vector<TexCoord> &RigidObject::getTexCoords() const
{
	return m_texcoords;
}

/**
 * TODO
 * @param index TODO
 * @return TODO
 */
const Position *RigidObject::getPosition(unsigned int index) const
{
	if (m_positions.empty() || index >= m_positions.size())
		return nullptr;
	return &m_positions[index];
}

/**
 * TODO
 * @param index TODO
 * @return TODO
 */
const Normal *RigidObject::getNormal(unsigned int index) const
{
	if (m_normals.empty() || index >= m_normals.size())
		return nullptr;
	return &m_normals[index];
}

/**
 * TODO
 * @param index TODO
 * @return TODO
 */
const TexCoord *RigidObject::getTexCoord(unsigned int index) const
{
	if (m_texcoords.empty() || index >= m_texcoords.size())
		return nullptr;
	return &m_texcoords[index];
}

/**
 * TODO
 * @return TODO
 */
int RigidObject::getPositionIndex(Position position) const
{
	for (unsigned int i = 0; i < m_positions.size(); i++)
	{
		if (position.x == m_positions[i].x &&
			position.y == m_positions[i].y &&
			position.z == m_positions[i].z)
			return (i);
	}
	return (-1);
}

/**
 * TODO
 * @return TODO
 */
int RigidObject::getNormalIndex(Normal normal) const
{
	for (unsigned int i = 0; i < m_normals.size(); i++)
	{
		if (normal.x == m_normals[i].x &&
			normal.y == m_normals[i].y &&
			normal.z == m_normals[i].z)
			return (i);
	}
	return (-1);
}

/**
 * TODO
 * @return TODO
 */
int RigidObject::getTexCoordIndex(TexCoord texcoord) const
{
	for (unsigned int i = 0; i < m_texcoords.size(); i++)
	{
		if (texcoord.x == m_texcoords[i].x &&
			texcoord.y == m_texcoords[i].y)
			return (i);
	}
	return (-1);
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

/**
 * TODO
 * @param position TODO
 */
void RigidObject::addMesh(Mesh mesh)
{
	m_meshes.push_back(mesh);
}

/**
 * TODO
 * @param position TODO
 */
void RigidObject::addMaterial(Material material)
{
	m_materials.push_back(material);
}
/**
 * TODO
 * @param position TODO
 */
unsigned int RigidObject::addPosition(Position position)
{
	int rc;
	if ((rc = getPositionIndex(position)) < 0)
	{
		m_positions.push_back(position);
		return (m_positions.size()-1);
	}
	return (rc);
}

/**
 * TODO
 * @param position TODO
 */
unsigned int RigidObject::addNormal(Normal normal)
{
	int rc;
	if ((rc = getNormalIndex(normal)) < 0)
	{
		m_normals.push_back(normal);
		return (m_normals.size()-1);
	}
	return (rc);
}

/**
 * TODO
 * @param position TODO
 */
unsigned int RigidObject::addTexCoord(TexCoord texcoord)
{
	int rc;
	if ((rc = getTexCoordIndex(texcoord)) < 0)
	{
		m_texcoords.push_back(texcoord);
		return (m_texcoords.size()-1);
	}
	return (rc);
}
