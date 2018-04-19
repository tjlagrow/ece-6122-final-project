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
 * @param face TODO
 */
void Mesh::addFace(Face face)
{
	m_faces.push_back(face);
}

/**
 * TODO
 * @param face TODO
 */
void Mesh::addFace(
	unsigned int px, unsigned int py, unsigned int pz,
	unsigned int tx, unsigned int ty, unsigned int tz,
	unsigned int nx, unsigned int ny, unsigned int nz)
{
	m_faces.push_back(Face(px, py, pz, tx, ty, tz, nx, ny, nz));
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
