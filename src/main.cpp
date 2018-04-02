#include "ArgParser.h"
#include "Window.h"
#include "PhysicsEngine.h"
#include "Shader.h"
#include "Font.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#define VS_CUBE_PATH  "../shaders/vertex.glsl"
#define FS_CUBE_PATH  "../shaders/fragment.glsl"

#define WINDOW_WIDTH    640
#define WINDOW_HEIGHT   480
#define WINDOW_TITLE    "ECE6122 Final Project"

static GLfloat vertices[] =
{
	+0.0f, +0.0f, +1.0f,  1.0f, 0.0f, 0.0f, 1.0f, // back  lower left
	+1.0f, +0.0f, +1.0f,  1.0f, 0.0f, 0.0f, 1.0f, // back  lower right
	+1.0f, +1.0f, +1.0f,  0.0f, 1.0f, 0.0f, 1.0f, // back  upper right
	+0.0f, +1.0f, +1.0f,  0.0f, 1.0f, 0.0f, 1.0f, // back  upper left
	+0.0f, +0.0f, +0.0f,  0.0f, 0.0f, 1.0f, 1.0f, // front lower left
	+1.0f, +0.0f, +0.0f,  0.0f, 0.0f, 1.0f, 1.0f, // front lower right
	+1.0f, +1.0f, +0.0f,  1.0f, 1.0f, 0.0f, 1.0f, // front upper right
	+0.0f, +1.0f, +0.0f,  1.0f, 1.0f, 0.0f, 1.0f, // front upper left
};

static GLuint indices[] =
{
	0, 1, 2,  2, 3, 0, // back
	4, 5, 6,  6, 7, 4, // front
	1, 5, 6,  6, 2, 1, // right
	0, 4, 7,  7, 3, 0, // left
	2, 3, 7,  7, 6, 2, // top
	0, 1, 5,  5, 4, 0, // bottom
};

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

	Window window(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);

	Shader shader1(VS_CUBE_PATH, FS_CUBE_PATH);

//	PhysicsEngine engine;
//	engine.simple_ball_drop();

	GLuint vao, vbo, ibo;
	GLuint loc;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	shader1.enable();
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	loc = shader1.getAttribLocation("vpos");
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 7*sizeof(GLfloat), 0);
	glEnableVertexAttribArray(loc);

	loc = shader1.getAttribLocation("vcol");
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 7*sizeof(GLfloat), (void *)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(loc);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	shader1.disable();

	///////////////////////////////////////////////

	float aspect = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
	float fov = 45.0f;
	glm::mat4 pMatrix = glm::perspective(glm::radians(fov), aspect, 0.1f, 100.f);
	glm::mat4 vMatrix = glm::lookAt(glm::vec3(4.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 mMatrix = glm::mat4(1.0f);
	glm::mat4 mvpMatrix = pMatrix * vMatrix * mMatrix;

	Font font(VS_FONT_PATH, FS_FONT_PATH, TTF_PATH);

	double tn = window.get_time();
	double td = tn + 1.0f;
	size_t indices_to_draw = sizeof(indices)/sizeof(GLuint);

	while (! window.should_close())
	{
		window.clear();

		if ((tn = window.get_time()) > td)
		{
			td = tn + 1.0f;
		}

		shader1.enable();
		shader1.setUniformMat4("mvp", mvpMatrix);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, indices_to_draw, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		shader1.disable();

		font.write("This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));


		window.update();
	}

	shader1.disable();
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	return (0);
}
