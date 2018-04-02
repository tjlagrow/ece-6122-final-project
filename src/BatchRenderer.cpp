#include "BatchRenderer.h"

BatchRenderer::BatchRenderer(GLsizei nbytes, GLuint *indices, GLuint nindices)
	: m_nbytes(nbytes)
{
	GLint loc;

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glBufferData(GL_ARRAY_BUFFER, nbytes, 0, GL_DYNAMIC_DRAW);

	m_shader->enable();

	loc = m_shader->getAttribLocation("vpos");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Vertex::position));

	loc = m_shader->getAttribLocation("vuv");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Vertex::uv));

	loc = m_shader->getAttribLocation("vtid");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Vertex::tid));

	loc = m_shader->getAttribLocation("vcol");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Vertex::color));

	m_shader->disable();

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nindices, indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

BatchRenderer::~BatchRenderer()
{
	glDeleteBuffers(1, &m_ibo);
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}

void BatchRenderer::begin()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	m_vertices = (Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
}

void BatchRenderer::end()
{
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BatchRenderer::submit()
{

}

void BatchRenderer::render()
{
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

	glDrawElements(GL_TRIANGLES, m_index_count, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_index_count = 0;
}
