#ifndef ECE_6122_FINAL_PROJECT_BUFFER_H
#define ECE_6122_FINAL_PROJECT_BUFFER_H


#include <GL/glew.h>

class Buffer
{
public:
	Buffer(GLfloat *data, GLsizei num_elements, GLuint num_components);
	~Buffer();

	void bind() const;
	void unbind() const;

	inline GLuint getNumComponents() const { return m_num_components; }

private:
	GLuint m_id;
	GLuint m_num_components;
};


#endif //ECE_6122_FINAL_PROJECT_BUFFER_H
