#include <map>
#include <algorithm>
#include "Renderer.h"
#include "meshes/Color.h"

/**
 * Constructor
 */
Renderer::Renderer()
{
	m_vao = 0;
	m_vbo = 0;
	m_ibo = 0;
	m_numVertices = 0;
	m_numIndices = 0;
	m_transformations.push_back(glm::mat4());
	m_back_transform = &m_transformations.back();

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, MESH_MAX_BUFFER_BYTES, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(SHADER_INDEX_POSITION);
	glEnableVertexAttribArray(SHADER_INDEX_UV);
	glEnableVertexAttribArray(SHADER_INDEX_COLOR);
	glEnableVertexAttribArray(SHADER_INDEX_NORMAL);

	glVertexAttribPointer(
		SHADER_INDEX_POSITION,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void *)(offsetof(Vertex, Vertex::position))
	);
	glVertexAttribPointer(
		SHADER_INDEX_UV,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void *)(offsetof(Vertex, Vertex::uv))
	);
	glVertexAttribPointer(
		SHADER_INDEX_COLOR,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void *)(offsetof(Vertex, Vertex::color)));
	glVertexAttribPointer(
		SHADER_INDEX_NORMAL,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void *)(offsetof(Vertex, Vertex::normal)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, MESH_MAX_INDICES, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

/**
 * Destructor
 */
Renderer::~Renderer()
{
	glDeleteBuffers(1, &m_ibo);
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}

/**
 * Generates element array buffers for storing indices
 * @param count The number of buffers to generate
 */
void Renderer::addMeshes(unsigned int count)
{
	size_t n = m_indices.size();
	for (unsigned int i = n; i < n + count; ++i)
		m_indices.push_back({ });
}

/**
 * TODO
 * @param object TODO
 */
void Renderer::submit(const RigidObject *object)
{
	std::vector<Mesh> meshes = object->getMeshes();
//	std::vector<Material> materials = object->getMaterials();
//	std::vector<Position> positions = object->getPositions();
//	std::vector<Normal> normals = object->getNormals();
//	std::vector<TexCoord> texcoords = object->getTexCoords();

	std::vector<Vertex> vertices;

	// Loop over the meshes in this object and extract the relevant vertices
	// and matching index positions into their respective arrays. These will
	// be sent to the GPU later in this function.
	for (unsigned int i = 0; i < meshes.size(); ++i)
	{
		// Each mesh has a face
		for (auto &face : meshes[i].getFaces())
		{
			// Each face has 3 vertices
			for (unsigned int j = 0; j < 3; j++)
			{
				Position pos;
				TexCoord tex;
				Color col;
				Normal nrm;

				const Position *ptrPosition = object->getPosition(face.positionIndices[j]);
				if (!ptrPosition)
					continue;
				pos = *ptrPosition;

				const TexCoord *ptrTexCoord = object->getTexCoord(face.texcoordIndices[j]);
				if (! ptrTexCoord)
					tex = TexCoord();
				else
					tex = *ptrTexCoord;

				col = Color(1.0f, 1.0f, 1.0f, 1.0f);

				const Normal *ptrNormal = object->getNormal(face.normalIndices[j]);
				if (! ptrNormal)
					nrm = Normal();
				else
					nrm = *ptrNormal;

				Vertex v(pos.getGlmVec4(), tex, col, nrm);
				unsigned int index = 0;

				if (! vertices.empty())
				{
					// Look and see if this vertex already exists in our array
					// If so, no need to add it and duplicate it. That will waste
					// space and time on the GPU. Instead, we'll just make note
					// and store the index of the existing Vertex
					// NOTE: If this thing below looks weird it's because it
					// is kind of weird. It's called a "lambda" function.
					auto it = std::find_if(vertices.begin(), vertices.end(),
						[&v](const Vertex &obj) // lambda function begin
						{
							return (
								obj.position.x == v.position.x &&
								obj.position.y == v.position.y &&
								obj.position.z == v.position.z &&
								obj.color.r == v.color.r &&
								obj.color.g == v.color.g &&
								obj.color.b == v.color.b &&
								obj.color.a == v.color.a &&
								obj.normal.x == v.normal.x &&
								obj.normal.y == v.normal.y &&
								obj.normal.z == v.normal.z &&
								obj.uv.x == v.uv.x &&
								obj.uv.y == v.uv.y
							);
						}
					);

					// Check if we found the object
					if (it != vertices.end())
					{
						// We found it. Grab the index
						index = std::distance(vertices.begin(), it);
					}
					else
					{
						// Did not find it. New index is the last element + 1
						index = vertices.size();
						vertices.push_back(v);
					}
					m_indices[i].push_back(index);
				}
				else
				{
					vertices.push_back(v);
					m_indices[i].push_back(index);
				}
			}
		}
		m_numIndices += m_indices[i].size();
	}

	GLsizei vertBytes = vertices.size() * sizeof(Vertex);
	GLintptr vertOffset = m_numVertices * sizeof(Vertex);

	GLsizei indicesBytes = m_indices.back().size() * sizeof(GLuint);
	GLintptr indicesOffset = m_numIndices * sizeof(GLuint);
//
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, vertOffset, vertBytes, vertices.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indicesOffset, indicesBytes, m_indices.back().data());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_numVertices += vertices.size();
	printf("%u %u\n", m_numIndices, m_numVertices);
}

/**
 * Draws the triangles in this batch to the back buffer
 */
void Renderer::flush()
{
	unsigned int count = 0;
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	for (unsigned int i = 0; i < m_indices.size(); ++i)
	{
		glDrawElements(GL_TRIANGLES, m_indices[i].size(), GL_UNSIGNED_INT, (void *)(count * sizeof(GLuint)));
		count += m_indices[i].size();
		m_indices[i].clear();
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_numVertices = 0;
	m_numIndices = 0;
}

/**
 * Pushes a transformation matrix. For more information, see:
 * <https://www.youtube.com/watch?v=16GtFaXwMSo&index=17&list=PLlrATfBNZ98fqE45g3jZA_hLGUrD4bo6_>
 * @param matrix TODO
 * @param override TODO
 */
void Renderer::push(glm::mat4 matrix, bool override)
{
	if (override)
		m_transformations.push_back(matrix);
	else
		// For more information, see:
		m_transformations.push_back(m_transformations.back() * matrix);

	m_back_transform = &m_transformations.back();
}

/**
 * Pops a transformation matrix
 */
void Renderer::pop()
{
	if (m_transformations.size() > 1)
		m_transformations.pop_back();

	m_back_transform = &m_transformations.back();
}
