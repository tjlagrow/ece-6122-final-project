#include "ArgParser.h"
#include "Window.h"
#include "PhysicsEngine.h"
#include "Shader.h"
#include "Camera.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#define VERTEX_PATH    "../shaders/vertex.glsl"
#define FRAGMENT_PATH  "../shaders/fragment.glsl"

static int width = 640;
static int height = 480;
static const char *title = "ECE6122 Final Project";

/**
 * Main program entry point
 * @param argc Number of command-line arguments
 * @param argv Command line arguments array
 * @return Returns 0 on success, else error
 */
int main(int argc, char **argv)
{
	ProgramConfig cfg;
	ArgParser::parse_args(argc, argv, &cfg);

	Window window(title, width, height);

	Shader shader(VERTEX_PATH, FRAGMENT_PATH);
	shader.enable();

	GLfloat vertices[] = {
		+0.0f, +0.0f, +1.0f,  1.0f, 0.0f, 0.0f, 1.0f, // back  lower left
		+1.0f, +0.0f, +1.0f,  1.0f, 0.0f, 0.0f, 1.0f, // back  lower right
		+1.0f, +1.0f, +1.0f,  0.0f, 1.0f, 0.0f, 1.0f, // back  upper right
		+0.0f, +1.0f, +1.0f,  0.0f, 1.0f, 0.0f, 1.0f, // back  upper left
		+0.0f, +0.0f, +0.0f,  0.0f, 0.0f, 1.0f, 1.0f, // front lower left
		+1.0f, +0.0f, +0.0f,  0.0f, 0.0f, 1.0f, 1.0f, // front lower right
		+1.0f, +1.0f, +0.0f,  1.0f, 1.0f, 0.0f, 1.0f, // front upper right
		+0.0f, +1.0f, +0.0f,  1.0f, 1.0f, 0.0f, 1.0f, // front upper left
	};
	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0,
		4, 5, 6,
		6, 7, 4,
		1, 5, 6,
		6, 2, 1,
		0, 4, 7,
		7, 3, 0

	};

	GLuint vao, vbo, ibo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7*sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7*sizeof(GLfloat), (void *)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

//	PhysicsEngine engine;
//	engine.simple_ball_drop();

	size_t indices_to_draw = sizeof(indices)/sizeof(GLuint);
	float aspect = (float)width / (float)height;
	Camera cam(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 projectionMatrix = glm::perspective(60.0f, aspect, 0.1f, 10.0f);
	glm::mat4 fullTransforms[] =
	{
		projectionMatrix * cam.get_view() * glm::translate(glm::vec3(-1.0f, 0.0f, -3.0f)) * glm::rotate(36.0f, glm::vec3(1.0f, 0.0f, 0.0f)),
		projectionMatrix * cam.get_view() * glm::translate(glm::vec3(+1.0f, 0.0f, -3.8f)) * glm::rotate(126.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
	};


	while (! window.should_close())
	{
		window.clear();

		window.get_cursor_pos();

		shader.setUniformMat4("transform_matrix", fullTransforms[0]);
		glDrawElements(GL_TRIANGLES, indices_to_draw, GL_UNSIGNED_INT, 0);

		shader.setUniformMat4("transform_matrix", fullTransforms[1]);
		glDrawElements(GL_TRIANGLES, indices_to_draw, GL_UNSIGNED_INT, 0);

		window.update();
	}

	shader.disable();
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	return (0);
}
