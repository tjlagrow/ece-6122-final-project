#ifndef ECE_6122_FINAL_PROJECT_BATCHRENDERER_H
#define ECE_6122_FINAL_PROJECT_BATCHRENDERER_H

#include "Shape.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#define RENDERER_MAX_SHAPES          1000
#define RENDERER_MAX_INDICES         10000
#define RENDERER_MAX_BUFFER_BYTES    (100 * sizeof(Vertex) * RENDERER_MAX_SHAPES)

#define SHADER_INDEX_POSITION    0
#define SHADER_INDEX_UV          1
#define SHADER_INDEX_COLOR       2

class Shape;

class BatchRenderer
{
public:
	BatchRenderer();
	~BatchRenderer();

	void submit(const Shape *shape);
	void flush();

	void push(glm::mat4 matrix, bool override = false);
	void pop();

private:

	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ibo;
	GLsizei m_num_vertices;
	GLsizei m_num_indices;

	std::vector<glm::mat4> m_transformations;
	const glm::mat4 *m_back_transform;

};


#endif //ECE_6122_FINAL_PROJECT_BATCHRENDERER_H
