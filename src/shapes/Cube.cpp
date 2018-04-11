#include "Cube.h"
#include "../Vertex.h"
#include <iostream>

static std::vector<Vertex> vertices =
{ //  X      Y      Z      R     G     B     A
	{ { +0.0f, +0.0f, +1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, // back  lower left
	{ { +1.0f, +0.0f, +1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, // back  lower right
	{ { +1.0f, +1.0f, +1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, // back  upper right
	{ { +0.0f, +1.0f, +1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, // back  upper left
	{ { +0.0f, +0.0f, +0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, // front lower left
	{ { +1.0f, +0.0f, +0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, // front lower right
	{ { +1.0f, +1.0f, +0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } }, // front upper right
	{ { +0.0f, +1.0f, +0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } }, // front upper left
};

static std::vector<GLuint> indices =
{
	0, 1, 2,  2, 3, 0, // back
	4, 5, 6,  6, 7, 4, // front
	1, 5, 6,  6, 2, 1, // right
	0, 4, 7,  7, 3, 0, // left
	2, 3, 7,  7, 6, 2, // top
	0, 1, 5,  5, 4, 0, // bottom
};

/**
 * Constructor
 */
Cube::Cube() : Shape(vertices, indices)
{
}

/**
 * Destructor
 */
Cube::~Cube()
{
}