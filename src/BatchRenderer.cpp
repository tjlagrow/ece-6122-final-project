#include <iostream>
#include "BatchRenderer.h"

BatchRenderer::BatchRenderer() : m_index_count(0)
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_BYTES, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(SHADER_INDEX_POSITION);
	glEnableVertexAttribArray(SHADER_INDEX_COLOR);

	glVertexAttribPointer(
		SHADER_INDEX_POSITION,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void *)(offsetof(Vertex, Vertex::position))
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
	glBindBuffer(1, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, RENDERER_MAX_INDICES, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

BatchRenderer::~BatchRenderer()
{
	std::cout << __func__ << std::endl;
	glDeleteBuffers(1, &m_ibo);
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}

void BatchRenderer::begin()
{
//	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
//	m_buf = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
//	m_ind = (GLuint *)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
}

void BatchRenderer::end()
{
//	glUnmapBuffer(GL_ARRAY_BUFFER);
//	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BatchRenderer::submit(const Shape *shape)
{
	std::vector<Vertex> vertices = shape->getVertices();
	std::vector<GLuint> indices = shape->getIndices();

	GLsizei verticesBytes = vertices.size() * sizeof(Vertex);
	GLsizei indicesBytes = indices.size() * sizeof(GLuint);

	std::cout << "verts count: " << vertices.size() << std::endl;
	std::cout << "verts bytes: " << verticesBytes << std::endl;
	std::cout << "inds count: " << indices.size() << std::endl;
	std::cout << "inds bytes: " << indicesBytes << std::endl;

//	for (unsigned int i = 0; i < vertices.size(); ++i)
//	{
//		vertices[i].position =
//			glm::vec3(*m_back_transform * glm::vec4(vertices[i].position, 1.0));
//	}

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, verticesBytes, vertices.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indicesBytes, indices.data());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_index_count += indices.size();
}

/**
 * Draws the triangles in this batch to the back buffer
 */
void BatchRenderer::flush()
{
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glDrawElements(GL_TRIANGLES, m_index_count, GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_index_count = 0;
}