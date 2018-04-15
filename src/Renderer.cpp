#include "Renderer.h"

/**
 * Constructor
 */
Renderer::Renderer()
{
	m_vao = 0;
	m_vbo = 0;
	m_ibo = 0;
	m_num_vertices = 0;
	m_num_indices = 0;
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
 * @param shape TODO
 */
void Renderer::submit(const Mesh *shape)
{
	std::vector<Vertex> verts = shape->getVertices();
	std::vector<Face> faces = shape->getFaces();
	glm::mat4 mt = shape->getModelTransform();

	GLsizei verticesBytes = verts.size() * sizeof(Vertex);
	GLsizei indicesBytes = faces.size() * 3 * sizeof(GLuint);
	GLintptr verticesOffset = m_num_vertices * sizeof(Vertex);
	GLintptr indicesOffset = m_num_indices * sizeof(GLuint);

	// Apply transformations to vertices
	for (unsigned int i = 0; i < verts.size(); i++)
	{
		// TODO I don't know if applying these two transforms is a good idea
		// You definitely need to apply the model transform, but the other?
		verts[i].position = *m_back_transform * mt * verts[i].position;
	}

	// Apply offsets to indices
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		faces[i].indices[0] += m_num_vertices;
		faces[i].indices[1] += m_num_vertices;
		faces[i].indices[2] += m_num_vertices;
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, verticesOffset, verticesBytes, verts.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indicesOffset, indicesBytes, faces.data());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_num_indices += faces.size() * 3;
	m_num_vertices += verts.size();
}

/**
 * Draws the triangles in this batch to the back buffer
 */
void Renderer::flush()
{
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glDrawElements(GL_TRIANGLES, m_num_indices, GL_UNSIGNED_INT, nullptr);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_num_vertices = 0;
	m_num_indices = 0;
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
