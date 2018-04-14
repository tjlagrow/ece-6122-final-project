#include "ArgParser.h"
#include "Window.h"
#include "PhysicsEngine.h"
#include "Shader.h"
#include "TextWriter.h"
#include "shapes/Cube.h"
#include "Layer.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

////////////////////////////////////////////////////////
// BEGIN - GLOBAL VARIABLES FOR TEST ONLY
////////////////////////////////////////////////////////
#define VS_CUBE_PATH  "../shaders/vertex.glsl"
#define FS_CUBE_PATH  "../shaders/fragment.glsl"

#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   600
#define WINDOW_TITLE    "ECE6122 Final Project"

static float camx = 4.0f;
static float camy = 3.0f;
static float camz = 3.0f;

////////////////////////////////////////////////////////
// END - GLOBAL VARIABLES FOR TEST ONLY
////////////////////////////////////////////////////////

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

	TextWriter tw(VS_FONT_PATH, FS_FONT_PATH, TTF_PATH, 16);

//	PhysicsEngine engine;
//	engine.simple_ball_drop();

////////////////////////////////////////////////////////
// BEGIN - TODO ABSTRACT THIS STUFF
////////////////////////////////////////////////////////

	Shader shader1(VS_CUBE_PATH, FS_CUBE_PATH);

	Cube cube1(glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 2.0f)));
	Cube cube2;

	Layer layer1(&shader1);
	layer1.add(&cube1);
	layer1.add(&cube2);

	float aspect = (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT;
	float fov = 45.0f;
	glm::mat4 pMatrix = glm::perspective(
		glm::radians(fov),
		aspect,
		0.1f,
		100.f
	);
	glm::mat4 vMatrix = glm::lookAt(
		glm::vec3(camx, camy, camz), // camera position
		glm::vec3(0.0f, 0.0f, 0.0f), // where do you look
		glm::vec3(0.0f, 1.0f, 0.0f)  // y-axis is up orientation
	);
	glm::mat4 mMatrix1 = glm::translate(glm::vec3(+0.0f, +0.0f, +2.0f));
	glm::mat4 mMatrix2 = glm::translate(glm::vec3(+0.0f, +0.0f, -2.0f));

	// Frames per second stuff
	size_t frames = 0;
	char frm_str[32] = "frames: 0";
	char sec_str[32] = "secs:   0.000";
	char fps_str[32] = "fps:    0.000";
	double tnow = window.get_time();
	double tnext = tnow + 1.0f;
	float sx = 2.0f / window.get_width();
	float sy = 2.0f / window.get_height();
	glm::vec4 color_white(1.0f, 1.0f, 1.0f, 1.0f);

////////////////////////////////////////////////////////
// END - TODO ABSTRACT THIS STUFF
////////////////////////////////////////////////////////

	while (! window.should_close())
	{
		window.clear();

		if ((tnow = window.get_time()) > tnext)
		{
			memset(frm_str, 0, sizeof(frm_str));
			memset(sec_str, 0, sizeof(sec_str));
			memset(fps_str, 0, sizeof(fps_str));
			snprintf(frm_str, sizeof(frm_str)-1, "frames: %lu", frames);
			snprintf(sec_str, sizeof(sec_str)-1, "secs:   %06.3f", tnow);
			snprintf(fps_str, sizeof(fps_str)-1, "fps:    %06.3f", frames / tnow);
			tnext = tnow + 1.0f;
		}

		shader1.enable();
		shader1.setUniformMat4("vpmat", pMatrix);
		shader1.setUniformMat4("vvmat", vMatrix);
		shader1.setUniformMat4("vmmat", mMatrix2);
		layer1.render();
		shader1.disable();
//		glBindVertexArray(vao);
//		glDrawElements(GL_TRIANGLES, cube.getNumIndices(), GL_UNSIGNED_INT, 0);
//		glBindVertexArray(0);

		tw.begin();
		tw.write(frm_str, -0.99f, +0.95f, sx, sy, color_white);
		tw.write(sec_str, -0.99f, +0.90f, sx, sy, color_white);
		tw.write(fps_str, -0.99f, +0.85f, sx, sy, color_white);
		tw.end();

		window.update();
		frames++;
	}

	return (0);
}
