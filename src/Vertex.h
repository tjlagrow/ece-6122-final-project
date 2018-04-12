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

};

std::ostream &operator<< (std::ostream &stream, const Vertex &vertex);

#endif //ECE_6122_FINAL_PROJECT_VERTEX_H
