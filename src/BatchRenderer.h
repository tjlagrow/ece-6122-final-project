#ifndef ECE_6122_FINAL_PROJECT_BATCHRENDERER_H
#define ECE_6122_FINAL_PROJECT_BATCHRENDERER_H


#include "Shader.h"
#include "Vertex.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

class BatchRenderer
{
public:
	BatchRenderer(GLsizei nbytes, GLuint *indices, GLuint nindices);
	~BatchRenderer();

	void begin();
	void end();
	void submit();
	void render();

	void push(glm::mat4 matrix);
	void pop();

private:
	void init();

	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ibo;
	GLsizei m_index_count;
	GLsizei m_nbytes;
	Vertex *m_vertices;

	Shader *m_shader;

};


#endif //ECE_6122_FINAL_PROJECT_BATCHRENDERER_H
