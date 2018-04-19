#include "Vertex.h"

/**
 * Constructor
 */
Vertex::Vertex()
{
	position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	uv = glm::vec2(1.0f, 1.0f);
	color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	normal = glm::vec3(0.0f, 0.0f, 0.0f);
}

/**
 * Constructor
 * @param position The vertex position in model space
 * @param uv The texture coordinate mapping for this vertex
 * @param color The color of this vertex
 * @param normal The normal for this vertex
 */
Vertex::Vertex(glm::vec4 position, glm::vec2 uv, glm::vec4 color, glm::vec3 normal)
{
	this->position = position;
	this->uv = uv;
	this->color = color;
	this->normal = normal;
}

/**
 * Constructor
 * @param x Position in x-direction
 * @param y Position in y-direction
 * @param z Position in z-direction
 * @param w 1 means a point, 0 means a vector (usually=1)
 * @param u Texture coordinate in x-direction
 * @param v Texture coordinate in y-direction
 * @param r Red color value
 * @param g Green color value
 * @param b Blue color value
 * @param a Alpha value
 * @param m Normal coordinate in x-direction
 * @param n Normal coordinate in y-direction
 * @param o Normal coordinate in z-direction
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

/**
 * Overload of the << operator for ease of printing the values
 * @param stream The ostream to write to (i.e. std::cout)
 * @param vertex The vertex to print
 * @return Returns the original ostream
 */
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
