#ifndef ECE_6122_FINAL_PROJECT_VERTEX_H
#define ECE_6122_FINAL_PROJECT_VERTEX_H


#include <glm/glm.hpp>
#include <iostream>

struct Vertex
{
	glm::vec4 position;
	glm::vec2 uv;
//	GLuint tid;
	glm::vec4 color;
	glm::vec3 normal;

	Vertex();
	Vertex(glm::vec4 position, glm::vec2 uv, glm::vec4 color, glm::vec3 normal);
	Vertex(
		float x, float y, float z, float w,
		float u, float v,
		float r, float g, float b, float a,
		float m, float n, float o);
};

std::ostream &operator<< (std::ostream &stream, const Vertex &vertex);

#endif //ECE_6122_FINAL_PROJECT_VERTEX_H
