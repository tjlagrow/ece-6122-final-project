#include "Buffer.h"

/**
 * Constructor
 * @param data TODO
 * @param num_elements TODO
 * @param num_components TODO
 */
Buffer::Buffer(GLfloat *data, GLsizei num_elements, GLuint num_components)
	: m_num_components(num_components)
{
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glBufferData(GL_ARRAY_BUFFER, num_elements * sizeof(GLfloat), data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/**
 * Desctructor
 */
Buffer::~Buffer()
{
	glDeleteBuffers(1, &m_id);
}

/**
 * Binds the buffer (makes it active)
 */
void Buffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

/**
 * Unbinds the buffer (makes it inactive)
 */
void Buffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
