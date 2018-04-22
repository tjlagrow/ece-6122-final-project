#include "Object.h"
#include "ObjectLoader.h"

/**
 * Constructor
 */
Object::Object()
{
	m_mass = 1;
	m_inertia = glm::vec3(0.0f, 0.0f, 0.0f);
}

/**
 * Constructor
 * @param filepath TODO Document
 */
Object::Object(const char *filepath)
{
	ObjectLoader::loadFromFile(filepath, this);

	float maxx = 0, minx = 0, maxy = 0, miny = 0, maxz = 0, minz = 0;
	for (auto &mesh : m_meshes)
	{
		for (auto &face : mesh.getFaces())
		{
			if (m_positions[face.positionIndices.x].x	> maxx)
				maxx = m_positions[face.positionIndices.x].x;
			else if (m_positions[face.positionIndices.x].x < minx)
				minx = m_positions[face.positionIndices.x].x;

			if (m_positions[face.positionIndices.y].y	> maxy)
				maxy = m_positions[face.positionIndices.y].y;
			else if (m_positions[face.positionIndices.y].y < miny)
				miny = m_positions[face.positionIndices.y].y;

			if (m_positions[face.positionIndices.z].z	> maxz)
				maxz = m_positions[face.positionIndices.z].z;
			else if (m_positions[face.positionIndices.z].z < minz)
				minz = m_positions[face.positionIndices.z].z;
		}
	}
	m_size = glm::vec3(maxx-minx, maxy-miny, maxz-minz);

}

/**
 * Destructor
 */
Object::~Object()
{

}

/**
 * This function simply submits this shape to its renderer.
 * It is up to the renderer to actually draw it
 * @param renderer The renderer to use for drawing
 */
void Object::submit(Renderer *renderer) const
{
	renderer->submit(this);
}

/**
 * Get the meshes that make up this object
 * @return Returns this object's meshes
 */
const std::vector<Mesh> &Object::getMeshes() const
{
	return m_meshes;
}

/**
 * TODO Document
 * @return TODO Document
 */
const std::vector<Material> &Object::getMaterials() const
{
	return m_materials;
}

/**
 * TODO Document
 * @return TODO Document
 */
const std::vector<Position> &Object::getPositions() const
{
	return m_positions;
}

/**
 * TODO Document
 * @return TODO Document
 */
const std::vector<Normal> &Object::getNormals() const
{
	return m_normals;
}

/**
 * TODO Document
 * @return TODO Document
 */
const std::vector<TexCoord> &Object::getTexCoords() const
{
	return m_texcoords;
}

/**
 * TODO Document
 * @return TODO Document
 */
const glm::vec3 &Object::getOrigin() const
{
	return m_origin;
}

/**
 * TODO Document
 * @return TODO Document
 */
const float &Object::getMass() const
{
	return m_mass;
}

/**
 * TODO Document
 * @return TODO Document
 */
const glm::vec3 &Object::getInertia() const
{
	return m_inertia;
}

/**
 * TODO Document
 * @return TODO Document
 */
const glm::vec3 &Object::getSize() const
{
	return m_size;
}

/**
 * TODO Document
 * @param index TODO Document
 * @return TODO Document
 */
const Position *Object::getPosition(unsigned int index) const
{
	if (m_positions.empty() || index >= m_positions.size())
		return nullptr;
	return &m_positions[index];
}

/**
 * TODO Document
 * @param index TODO Document
 * @return TODO Document
 */
const Normal *Object::getNormal(unsigned int index) const
{
	if (m_normals.empty() || index >= m_normals.size())
		return nullptr;
	return &m_normals[index];
}

/**
 * TODO Document
 * @param index TODO Document
 * @return TODO Document
 */
const TexCoord *Object::getTexCoord(unsigned int index) const
{
	if (m_texcoords.empty() || index >= m_texcoords.size())
		return nullptr;
	return &m_texcoords[index];
}

/**
 * TODO Document
 * @return TODO Document
 */
int Object::getPositionIndex(Position position) const
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
 * TODO Document
 * @return TODO Document
 */
int Object::getNormalIndex(Normal normal) const
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
 * TODO Document
 * @return TODO Document
 */
int Object::getTexCoordIndex(TexCoord texcoord) const
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
 * TODO Document
 * @param origin TODO Document
 */
void Object::setOrigin(const glm::vec3 &origin)
{
	m_origin = origin;
}

/**
 * TODO Document
 * @return TODO Document
 */
void Object::setMass(const float &mass)
{
	m_mass = mass;
}

/**
 * TODO Document
 * @return TODO Document
 */
void Object::setInertia(const glm::vec3 &inertia)
{
	m_inertia = inertia;
}

/**
 * TODO Document
 * @param transform TODO Document
 */
void Object::applyTransform(glm::mat4 transform)
{
	for (auto &mesh : m_meshes)
		mesh.setModelTransform(transform);
}

/**
 * TODO Document
 * @param position TODO Document
 */
void Object::addMesh(Mesh mesh)
{
	m_meshes.push_back(mesh);
}

/**
 * TODO Document
 * @param position TODO Document
 */
void Object::addMaterial(Material material)
{
	m_materials.push_back(material);
}
/**
 * TODO Document
 * @param position TODO Document
 */
unsigned int Object::addPosition(Position position)
{
	int rc;
	if ((rc = getPositionIndex(position)) < 0)
	{
		m_positions.push_back(position);
		return (m_positions.size()-1);
	}
	return ((unsigned int)rc);
}

/**
 * TODO Document
 * @param position TODO Document
 */
unsigned int Object::addNormal(Normal normal)
{
	int rc;
	if ((rc = getNormalIndex(normal)) < 0)
	{
		m_normals.push_back(normal);
		return (m_normals.size()-1);
	}
	return ((unsigned int)rc);
}

/**
 * TODO Document
 * @param position TODO Document
 */
unsigned int Object::addTexCoord(TexCoord texcoord)
{
	int rc;
	if ((rc = getTexCoordIndex(texcoord)) < 0)
	{
		m_texcoords.push_back(texcoord);
		return (m_texcoords.size()-1);
	}
	return ((unsigned int)rc);
}
