#include "ArgParser.h"
#include "Window.h"
#include "PhysicsEngine.h"
#include "Shader.h"
#include "TextWriter.h"
#include "Layer.h"
#include "meshes/Mesh.h"
#include "meshes/ObjectLoader.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

////////////////////////////////////////////////////////
// BEGIN - GLOBAL VARIABLES FOR TEST ONLY
////////////////////////////////////////////////////////
#define VERTEX_SHADER_PATH    "../shaders/vertex.glsl"
#define FRAGMENT_SHADER_PATH  "../shaders/fragment.glsl"

#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   600
#define WINDOW_TITLE    "ECE6122 Final Project"

static float camx = 10.0f;
static float camy = 10.0f;
static float camz = 10.0f;
static float cama = 0.0f;
static float camr = 10.0f;

////////////////////////////////////////////////////////
// END - GLOBAL VARIABLES FOR TEST ONLY
////////////////////////////////////////////////////////

void calculateNewCameraPosition(float &x, float &y, float &a, float &r);

/**
 * Main program entry point
 * @param argc Number of command-line arguments
 * @param argv Command line arguments array
 * @return Returns 0 on success, else error
 */
int main(int argc, char **argv)
{
	// Parse program arguments and store in ProgramConfig
	// TODO Use ProgramConfig::modelsDir to automatically parse all models file
	ProgramConfig cfg;
	ArgParser::parse_args(argc, argv, &cfg);

	Window window(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);

	// The text writer is used to write text onto the window
	// It does not hinder the raytracer algorithm
	TextWriter tw(VS_FONT_PATH, FS_FONT_PATH, TTF_PATH, 16);

//	PhysicsEngine engine;
//	engine.simple_ball_drop();

	Shader shader1(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

	RigidObject beachball("../models/beachball.obj");
//	RigidObject cube1("../models/cube.obj");
//	RigidObject cube2("../models/cube_simple.obj");
//	RigidObject cube3("../models/cube_simple.obj");
//	RigidObject cube4("../models/cube.obj");
//	cube1.applyTransform(glm::translate(glm::vec3(+2.0f, +3.0f, +0.0f)));
//	cube2.applyTransform(glm::translate(glm::vec3(+0.0f, +0.0f, +0.0f)));
//	cube3.applyTransform(glm::translate(glm::vec3(-2.0f, -5.0f, +2.0f)));
//	cube4.applyTransform(glm::translate(glm::vec3(-5.0f, +0.0f, -3.0f)));

	Layer layer1(&shader1);
	layer1.add(&beachball);
//	layer1.add(&cube1);
//	layer1.add(&cube2);
//	layer1.add(&cube3);
//	layer1.add(&cube4);

////////////////////////////////////////////////////////
// BEGIN - TODO ABSTRACT THIS STUFF
////////////////////////////////////////////////////////

	// Set up the Projection matrix
	float aspect = (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT;
	float fov = 45.0f;
	glm::mat4 pMatrix = glm::perspective(
		glm::radians(fov),
		aspect,
		0.1f,
		100.f
	);

	// Set up the View matrix
	glm::mat4 vMatrix = glm::lookAt(
		glm::vec3(camx, camy, camz), // camera position
		glm::vec3(0.0f, 0.0f, 0.0f), // where do you look
		glm::vec3(0.0f, 1.0f, 0.0f)  // y-axis is up orientation
	);

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

	// Tell the vertex shader about the Projection and View matrices
	// The shader is looking for the Projection, View, and Model matrices
	// when it draws a vertex. The Projection and View matrices are handled
	// here; the Model matrix is handled by the individual Mesh objects
	// (like is the constructor for Cube class)
	shader1.enable();
	shader1.setUniformMat4("vpmat", pMatrix);
	shader1.setUniformMat4("vvmat", vMatrix);
	shader1.disable();

	while (! window.should_close())
	{
		// Clear the window's "back buffer" so
		// we can begin drawing a new scene
		window.clear();

		// If time has elapsed, get the diagnostic text to
		// overlay on the screen
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

		calculateNewCameraPosition(camx, camz, cama, camr);

		shader1.enable();
		vMatrix = glm::lookAt(
			glm::vec3(camx, camy, camz), // camera position
			glm::vec3(0.0f, 0.0f, 0.0f), // where do you look
			glm::vec3(0.0f, 1.0f, 0.0f)  // y-axis is up orientation
		);
		shader1.setUniformMat4("vvmat", vMatrix);
		shader1.disable();

		// Draw layers here
		layer1.render();

		// Draw text last so it is on top of the other layers
		tw.begin();
		tw.write(frm_str, -0.99f, +0.95f, sx, sy, color_white);
		tw.write(sec_str, -0.99f, +0.90f, sx, sy, color_white);
		tw.write(fps_str, -0.99f, +0.85f, sx, sy, color_white);
		tw.end();

		// Finally, update the window buffers to see what we've drawn
		window.update();
		frames++;
	}

	return (0);
}

void calculateNewCameraPosition(float &x, float &y, float &a, float &r)
{
	x = std::cos(a) * r;
	y = std::sin(a) * r;
	a += 0.01f;
}