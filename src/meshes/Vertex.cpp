#include "Vertex.h"

Vertex::Vertex()
{
	position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	uv = glm::vec2(1.0f, 1.0f);
	color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	normal = glm::vec3(0.0f, 0.0f, 0.0f);
}

/**
 * TODO
 * @param position TODO
 * @param uv TODO
 * @param color TODO
 */
Vertex::Vertex(glm::vec4 position, glm::vec2 uv, glm::vec4 color, glm::vec3 normal)
{
	this->position = position;
	this->uv = uv;
	this->color = color;
	this->normal = normal;
}

/**
 * TODO
 * @param x TODO
 * @param y TODO
 * @param z TODO
 * @param w TODO
 * @param u TODO
 * @param v TODO
 * @param r TODO
 * @param g TODO
 * @param b TODO
 * @param a TODO
 */
Vertex::Vertex(
	float x, float y, float z, float w, // Position
	float u, float v,                   // UV
	float r, float g, float b, float a, // Color
	float m, float n, float o)
{
	this->position = glm::vec4(x, y, z, w);
	this->uv = glm::vec2(u, v);
	this->color = glm::vec4(r, g, b, a);
	this->normal = glm::vec3(m, n, o);
}

std::ostream &operator<< (std::ostream &stream, const Vertex &vertex)
{
	stream
		<< vertex.position.x << " "
		<< vertex.position.y << " "
		<< vertex.position.z << " "
		<< vertex.position.w << ", "
		<< vertex.uv.s << " "
		<< vertex.uv.t << ", "
		<< vertex.color.r << " "
		<< vertex.color.g << " "
		<< vertex.color.b << " "
		<< vertex.color.a;
	return (stream);
}
