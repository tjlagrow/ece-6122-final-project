#include "Renderer.h"

/**
 * Constructor
 */
Renderer::Renderer()
{
	m_vao = 0;
	m_vbo = 0;
	m_ibo = 0;
	m_NumVertices = 0;
	m_NumIndices = 0;
	m_transformations.push_back(glm::mat4());
	m_back_transform = &m_transformations.back();

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, RENDERER_MAX_BUFFER_BYTES, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(SHADER_INDEX_POSITION);
	glEnableVertexAttribArray(SHADER_INDEX_UV);
	glEnableVertexAttribArray(SHADER_INDEX_COLOR);

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

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, RENDERER_MAX_INDICES, nullptr, GL_DYNAMIC_DRAW);
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
 * TODO
 * @param object TODO
 */
void Renderer::submit(const RigidObject *object)
{
	std::vector<Mesh> meshes = object->getMeshes();
	std::vector<Vertex> vertices;
	std::vector<Face> faces;

	for (auto &mesh : meshes)
	{
		Vertex v;
		glm::mat4 mt = mesh.getModelTransform();
		std::vector<Position> positions = mesh.getPositions();
		std::vector<Normal> normals = mesh.getNormals();

		for (unsigned int i = 0; i < positions.size(); ++i)
		{
//			v.position = *m_back_transform * mt * glm::vec4(positions[i].getGlmVec4());
			v.position = mt * glm::vec4(positions[i].getGlmVec4());
			v.normal = normals[i];
			v.color = Color().getGLM();

			vertices.push_back(v);
		}

		for (auto &f : mesh.getFaces())
		{
			faces.push_back(Face(
				f.x + vertices.size(),
				f.y + vertices.size(),
				f.z + vertices.size()
			));
		}
	}

	GLsizei verticesBytes = vertices.size() * sizeof(Vertex);
	GLsizei indicesBytes = faces.size() * 3 * sizeof(GLuint);
	GLintptr verticesOffset = m_NumVertices * sizeof(Vertex);
	GLintptr indicesOffset = m_NumIndices * sizeof(GLuint);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, verticesOffset, verticesBytes, vertices.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indicesOffset, indicesBytes, faces.data());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_NumIndices += faces.size() * 3;
	m_NumVertices += vertices.size();
}

/**
 * Draws the triangles in this batch to the back buffer
 */
void Renderer::flush()
{
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glDrawElements(GL_TRIANGLES, m_NumIndices, GL_UNSIGNED_INT, nullptr);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_NumVertices = 0;
	m_NumIndices = 0;
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
