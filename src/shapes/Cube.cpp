#include "Cube.h"
#include "../Vertex.h"
#include <iostream>


/**
 * Constructor
 * @param model_transform TODO
 */
Cube::Cube(glm::mat4 model_transform) : Shape(model_transform)
{
	m_vertices =
	{ //      X      Y      Z      W         U     V         R     G     B     A
		{ { -0.5f, -0.5f, +0.5f, +1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, // back  lower left
		{ { +0.5f, -0.5f, +0.5f, +1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, // back  lower right
		{ { +0.5f, +0.5f, +0.5f, +1.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, // back  upper right
		{ { -0.5f, +0.5f, +0.5f, +1.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, // back  upper left
		{ { -0.5f, -0.5f, -0.5f, +1.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, // front lower left
		{ { +0.5f, -0.5f, -0.5f, +1.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, // front lower right
		{ { +0.5f, +0.5f, -0.5f, +1.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } }, // front upper right
		{ { -0.5f, +0.5f, -0.5f, +1.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } }, // front upper left
	};

	m_indices =
	{
		0, 1, 2,  2, 3, 0, // back
		4, 5, 6,  6, 7, 4, // front
		1, 5, 6,  6, 2, 1, // right
		0, 4, 7,  7, 3, 0, // left
		2, 3, 7,  7, 6, 2, // top
		0, 1, 5,  5, 4, 0, // bottom
	};
}

Cube::Cube(glm::vec4 color, glm::mat4 model_transform) : Cube(model_transform)
{
	setColor(color);
}

/**
 * Destructor
 */
Cube::~Cube()
{
}

void Cube::setColor(const glm::vec4 &color)
{
	for (unsigned int i = 0; i < m_vertices.size(); i++)
		m_vertices[i].color = color;
}
