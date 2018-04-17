#include "Mesh.h"

/**
 * Constructor
 */
Mesh::Mesh()
{
}

/**
 * Destructor
 */
Mesh::~Mesh()
{
}

/**
 * TODO
 * @param position TODO
 */
void Mesh::addPosition(Position position)
{
	m_positions.push_back(position);
}

/**
 * TODO
 * @param normal TODO
 */
void Mesh::addNormal(Normal normal)
{
	m_normals.push_back(normal);
}

/**
 * TODO
 * @param face TODO
 */
void Mesh::addFace(Face face)
{
	m_faces.push_back(face);
}

/**
 * TODO
 * @param texcoord TODO
 */
void Mesh::addTexCoord(TexCoord texcoord)
{
	m_texcoords.push_back(texcoord);
}

/**
 * TODO
 * @param color TODO
 */
void Mesh::addColor(Color color)
{
	m_colors.push_back(color);
}

/**
 * TODO
 * @param materialIndex TODO
 */
void Mesh::setMaterialIndex(GLuint materialIndex)
{
	m_materialIndex = materialIndex;
}

/**
 * TODO
 * @param modelTransform TODO
 */
void Mesh::setModelTransform(glm::mat4 modelTransform)
{
	m_modelTransform = modelTransform;
}

/**
 * TODO
 * @return TODO
 */
const std::vector<Position> &Mesh::getPositions() const
{
	return m_positions;
}

/**
 * TODO
 * @return TODO
 */
const std::vector<Normal> &Mesh::getNormals() const
{
	return m_normals;
}

/**
 * Get the indices vector
 * @return A vector of indices for this shape
 */
const std::vector<Face> &Mesh::getFaces() const
{
	return m_faces;
}

/**
 * TODO
 * @return TODO
 */
const std::vector<TexCoord> &Mesh::getTexCoords() const
{
	return m_texcoords;
}

/**
 * TODO
 * @return TODO
 */
const std::vector<Color> &Mesh::getColors() const
{
	return m_colors;
}

/**
 * TODO
 * @return TODO
 */
const GLuint &Mesh::getMaterialIndex() const
{
	return m_materialIndex;
}

/**
 * Get the model transform matrix for the shape
 * @return The model transform matrix for this shape
 */
const glm::mat4 &Mesh::getModelTransform() const
{
	return m_modelTransform;
}
