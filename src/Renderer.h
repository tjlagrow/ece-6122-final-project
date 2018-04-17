#ifndef ECE_6122_FINAL_PROJECT_BATCHRENDERER_H
#define ECE_6122_FINAL_PROJECT_BATCHRENDERER_H

#include "meshes/Vertex.h"
#include "meshes/RigidObject.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#define RENDERER_MAX_INDICES         10000
#define RENDERER_MAX_BUFFER_BYTES    1000000

#define SHADER_INDEX_POSITION    0
#define SHADER_INDEX_UV          1
#define SHADER_INDEX_COLOR       2

class RigidObject;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void submit(const RigidObject *object);
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
