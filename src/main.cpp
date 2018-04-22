#include "ArgParser.h"
#include "Window.h"
#include "PhysicsEngine.h"
#include "Shader.h"
#include "TextWriter.h"
#include "Layer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

////////////////////////////////////////////////////////
// BEGIN - GLOBAL VARIABLES FOR TEST ONLY
////////////////////////////////////////////////////////

#define VERTEX_SHADER_PATH    "../shaders/vertex.glsl"
#define FRAGMENT_SHADER_PATH  "../shaders/fragment.glsl"

#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   600
#define WINDOW_TITLE    "ECE6122 Final Project"

static float camx = 10.0f; // Camera location x
static float camy = 10.0f; // Camera location y, this is the height
static float camz = 10.0f; // Camera location z
static float camr = 10.0f; // Camera radius
static float cama = 0.0f; // Camera angle in radians

// Delta multiplier for camera angle; affects speed of movement. Not exactly
// sure what units it's is, but a larger number means faster movement
static float camdt = 0.001f;

enum class ShapeType { Box, Sphere };

struct objMetadata
{
	bool isEnabled;
	ShapeType type;
	glm::vec3 position;
	float mass;
	const char *filepath;
};

static std::vector<struct objMetadata> objmeta =
{
	{ false, ShapeType::Sphere, glm::vec3(+0.0f, +0.0f, +0.0f), 1.0f, "../models/beachball.obj" },
	{ true,  ShapeType::Box,    glm::vec3(+0.0f, +9.0f, +0.0f), 1.0f, "../models/cube.obj" },
	{ true,  ShapeType::Box,    glm::vec3(+5.0f, +0.0f, +0.0f), 1.0f, "../models/cube.obj" },
	{ true,  ShapeType::Box,    glm::vec3(-5.0f, +0.0f, +0.0f), 1.0f, "../models/cube.obj" },
	{ true,  ShapeType::Box,    glm::vec3(+0.0f, +0.0f, +5.0f), 1.0f, "../models/cube.obj" },
	{ true,  ShapeType::Box,    glm::vec3(+0.0f, +0.0f, -5.0f), 1.0f, "../models/cube.obj" },
};

////////////////////////////////////////////////////////
// END - GLOBAL VARIABLES FOR TEST ONLY
////////////////////////////////////////////////////////

/**
 * Calculates a new camera position based on the current. The camera
 * is set up to rotate in a circle at a circle height
 * @param x The camera's x-position
 * @param z The camera's z-position
 * @param a The camera's angle (in radians)
 */
void updateCameraPosition(float &x, float &z, float &a)
{
	x = std::cos(a) * camr;
	z = std::sin(a) * camr;
	a += camdt;
}

/**
 * Main program entry point
 * @param argc Number of command-line arguments
 * @param argv Command line arguments array
 * @return Returns 0 on success, else error
 */
int main(int argc, char **argv)
{
	// Parse program arguments and store in ProgramConfig
	// TODO Use ProgramConfig::modelsDir to get all object files automatically
	ProgramConfig cfg;
	ArgParser::parse_args(argc, argv, &cfg);

	Window window(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);

	// The text writer is used to write text onto the window
	// It has no effect on the raytracing algorithm
	TextWriter tw(VS_FONT_PATH, FS_FONT_PATH, TTF_PATH, 16);

	// The physics engine is responsible for collision detection
	// and updating the objects' vertices each frame
	PhysicsEngine engine;

	Shader shader1(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	Layer layer1(&shader1);

	// Load the objects from file into the layer
	std::vector<Object *> objects;
	for (unsigned int i = 0; i < objmeta.size(); ++i)
	{
		if (! objmeta[i].isEnabled)
			continue;

		Object *o = new Object(objmeta[i].filepath);
		glm::mat4 transform = glm::translate(objmeta[i].position);
		o->applyTransform(transform);
		layer1.add(o);
		objects.push_back(o);
		switch (objmeta[i].type)
		{
			case ShapeType::Sphere:
				engine.addSphere(1.0f, objmeta[i].mass, objmeta[i].position);
				break;
			case ShapeType::Box:
//				engine.addBox(o->getSize(), objmeta[i].mass, objmeta[i].position);
				engine.addBox(glm::vec3(1.0f, 1.0f, 1.0f), objmeta[i].mass, objmeta[i].position);
				break;
		}
	}

////////////////////////////////////////////////////////
// BEGIN - TODO ABSTRACT THIS STUFF
////////////////////////////////////////////////////////

	// Set up the Projection matrix
	glm::mat4 pMatrix = glm::perspective(
		glm::radians(45.0f), // Field of view (zoom-in, zoom-out)
		(float) WINDOW_WIDTH / (float) WINDOW_HEIGHT, // Aspect ratio
		0.1f, // Near field cutoff
		100.f // Far field cutoff
	);

	// Set up the View matrix
	glm::mat4 vMatrix = glm::lookAt(
		glm::vec3(camx, camy, camz), // Camera position
		glm::vec3(0.0f, 0.0f, 0.0f), // Where do you look
		glm::vec3(0.0f, 1.0f, 0.0f)  // Y-axis is Up
	);

	// Frames per second stuff
	// TODO There's probably a better place to put this
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

		engine.stepSimulation(1 / 60.0f); // Speed up to like 1/24 if too slow
		std::vector<glm::vec3> positions;
		engine.getMotionStates(positions);

		// TODO Get transforms and update ALL the objects
		glm::mat4 updateTransform;
		Object *object = layer1.getObject(0);
		engine.getOpenGLMatrix(0, updateTransform);
		object->applyTransform(updateTransform);

		updateCameraPosition(camx, camz, cama);

		shader1.enable();
		vMatrix = glm::lookAt(
			glm::vec3(camx, camy, camz), // Camera position
			glm::vec3(0.0f, 0.0f, 0.0f), // Where do you look
			glm::vec3(0.0f, 1.0f, 0.0f)  // Y-axis is Up
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

	for (unsigned int i = 0; i < objects.size(); ++i)
	{
		delete objects[i];
	}

	return (0);
}

