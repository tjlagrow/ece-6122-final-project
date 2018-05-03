#include "ArgParser.h"
#include "Window.h"
#include "PhysicsEngine.h"
#include "Shader.h"
#include "TextWriter.h"
#include "Layer.h"
#include "utils/GlmUtils.h"
#include "raytracer/Raytracer.h"
#include "PerformanceData.h"
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

static float fov = 45.0f; // Filed of View in degrees
static float camx = -10.0f; // Camera location x
static float camy = 10.0f; // Camera location y, this is the height
static float camz = 20.0f; // Camera location z

enum class ShapeType { Box, Sphere, Cylinder, Cone, Custom };

struct objMetadata
{
	ShapeType type;
	glm::vec3 position;
	float mass;
	float bounciness;
	float friction;
	const char *filepath;
};

static std::vector<struct objMetadata> objmeta =
{
	{ ShapeType::Sphere,   glm::vec3(-7.0f, +30.0f, +0.0f), 10.f, 0.6f, 0.9f, "../models/ball_simple.obj" },
	{ ShapeType::Custom,   glm::vec3(-8.0f, +00.0f, +0.0f), 99.f, 0.6f, 0.9f, "../models/ramp.obj" },
	{ ShapeType::Box,      glm::vec3(+6.0f, +02.0f, -1.0f), 0.7f, 0.6f, 0.9f, "../models/cube.obj" },
	{ ShapeType::Box,      glm::vec3(+6.0f, +04.0f, -1.0f), 0.7f, 0.6f, 0.9f, "../models/cube.obj" },
	{ ShapeType::Box,      glm::vec3(+6.0f, +06.0f, -1.0f), 0.7f, 0.6f, 0.9f, "../models/cube.obj" },
	{ ShapeType::Box,      glm::vec3(+6.0f, +08.0f, -1.0f), 0.7f, 0.6f, 0.9f, "../models/cube.obj" },
	{ ShapeType::Box,      glm::vec3(+6.0f, +10.0f, -1.0f), 0.7f, 0.6f, 0.9f, "../models/cube.obj" },
	{ ShapeType::Box,      glm::vec3(+6.0f, +12.0f, -1.0f), 0.7f, 0.6f, 0.9f, "../models/cube.obj" },
	{ ShapeType::Box,      glm::vec3(+6.0f, +02.0f, +1.0f), 0.7f, 0.6f, 0.9f, "../models/cube.obj" },
	{ ShapeType::Box,      glm::vec3(+6.0f, +04.0f, +1.0f), 0.7f, 0.6f, 0.9f, "../models/cube.obj" },
	{ ShapeType::Box,      glm::vec3(+6.0f, +06.0f, +1.0f), 0.7f, 0.6f, 0.9f, "../models/cube.obj" },
	{ ShapeType::Box,      glm::vec3(+6.0f, +08.0f, +1.0f), 0.7f, 0.6f, 0.9f, "../models/cube.obj" },
	{ ShapeType::Box,      glm::vec3(+6.0f, +10.0f, +1.0f), 0.7f, 0.6f, 0.9f, "../models/cube.obj" },
	{ ShapeType::Box,      glm::vec3(+6.0f, +12.0f, +1.0f), 0.7f, 0.6f, 0.9f, "../models/cube.obj" },
//	{ ShapeType::Cone,     glm::vec3(+7.0f, +00.0f, +7.0f), 1.0f, 0.1f, 0.5f, "../models/cone.obj" },
//	{ ShapeType::Cone,     glm::vec3(-7.0f, +00.0f, -7.0f), 1.0f, 0.1f, 0.5f, "../models/cone.obj" },
//	{ ShapeType::Cylinder, glm::vec3(-7.0f, +00.0f, +7.0f), 1.0f, 0.1f, 0.5f, "../models/cylinder.obj" },
//	{ ShapeType::Cylinder, glm::vec3(+7.0f, +00.0f, -7.0f), 1.0f, 0.1f, 0.5f, "../models/cylinder.obj" },
};

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
	// Parse program arguments and store in ProgramConfig
	ProgramConfig cfg;
	ArgParser::parse_args(argc, argv, &cfg);

	if (cfg.verbose)
	{
		int pad = -15;
		printf("%*s: %s\n", pad, "Output file", cfg.output_filename);
		printf("%*s: %s\n", pad, "Raytrace", cfg.raytrace ? "yes" : "no");
		printf("%*s: %d\n", pad, "Threads", cfg.numThreads);
		printf("%*s: %d\n", pad, "Time", cfg.stopTime_s);
		printf("%*s: %s\n", pad, "Verbose", cfg.verbose ? "yes" : "no");
	}

	Window window(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);

	// The text writer is used to write text onto the window
	// It has no effect on the raytracing algorithm
	TextWriter tw(VS_FONT_PATH, FS_FONT_PATH, TTF_PATH, 16);

	// The physics engine is responsible for collision detection
	// and updating the objects' vertices each frame
	PhysicsEngine engine;

	// Initialize the raytracer
	Raytracer raytracer(window.getWidth(), window.getHeight(), fov);

	Shader shader1(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

	Layer stageLayer(&shader1);
	Layer modelLayer(&shader1);

	Object stage("../models/stage.obj");
	stageLayer.add(&stage);
	engine.addWall();

	// Load the objects from file into the layer
	std::vector<Object *> objects;
	for (unsigned int i = 0; i < objmeta.size(); ++i)
	{
		auto *o = new Object(objmeta[i].filepath);
		glm::mat4 transform = glm::translate(objmeta[i].position);
		o->setTransform(transform);
		modelLayer.add(o);
		objects.push_back(o);
		switch (objmeta[i].type)
		{
			case ShapeType::Sphere:
				engine.addSphere(
					1.0f,
					objmeta[i].mass,
					objmeta[i].bounciness,
					objmeta[i].friction,
					objmeta[i].position
				);
				break;
			case ShapeType::Box:
				engine.addBox(
					o->getSize()/2.0f, // Bullet wants half-lengths
					objmeta[i].mass,
					objmeta[i].bounciness,
					objmeta[i].friction,
					objmeta[i].position
				);
				break;
			case ShapeType::Cone:
				// TODO add to engine
				break;
			case ShapeType::Cylinder:
				// TODO add to engine
				break;
			case ShapeType::Custom:
				engine.addHull(
					o->getPositionsGlmVec3(),
					objmeta[i].mass,
					objmeta[i].bounciness,
					objmeta[i].friction,
					objmeta[i].position
				);
				break;
		}
	}

////////////////////////////////////////////////////////
// BEGIN - TODO Find a better home for this stuff
////////////////////////////////////////////////////////

	// Set up the Projection matrix
	glm::mat4 pMatrix = glm::perspective(
		glm::radians(fov), // Field of view (zoom-in, zoom-out)
		(float) WINDOW_WIDTH / (float) WINDOW_HEIGHT, // Aspect ratio
		0.1f, // Near field cutoff
		1000.f // Far field cutoff
	);

	// Set up the View matrix
	glm::mat4 vMatrix = glm::lookAt(
		glm::vec3(camx, camy, camz), // Camera position
		glm::vec3(0.0f, 0.0f, 0.0f), // Where do you look
		glm::vec3(0.0f, 1.0f, 0.0f)  // Y-axis is Up
	);

////////////////////////////////////////////////////////
// END - TODO Find a better home for this stuff
////////////////////////////////////////////////////////

	// Tell the vertex shader about the Projection and View matrices. The GPU
	// can make short work of multiplying these matrices. On the other hand,
	// the model matrix is computed on the CPU when the objects are rendered.
	// Sometimes in this code you'll see the model matrix called the
	// "transform" matrix (because it transforms the object's vertices from
	// local space to world space).
	shader1.enable();
	shader1.setUniformMat4("vpmat", pMatrix); // Vertexshader-Projection-MATrix
	shader1.setUniformMat4("vvmat", vMatrix); // Vertexshader-View-MATrix
	shader1.disable();

	// Mainly used for frames per second statistics
	// but could very easily be extended in the future
	PerformanceData perf;
	double elapsedTime;
	double sx = 2.0f / window.getWidth();
	double sy = 2.0f / window.getHeight();

	// Main loop, do everything here (render objects, update their positions,
	// collision detection, raytracing, performance statistics, etc.)
	while (! window.shouldClose())
	{
		// Clear the window's "back buffer" so
		// we can begin drawing a new scene
		window.clear();

		// If time has elapsed, get the diagnostic text to
		// overlay on the screen, but don't print it out here yet
		if ((elapsedTime = perf.getElapsedTime()) > 1.0f)
		{
			perf.updateStats(); // Update the performance statistics
			perf.reset();
		}

		engine.stepSimulation(1 / 60.0f); // Speed up to like 1/24 if too slow
		std::vector<glm::vec3> positions;
		engine.getMotionStates(positions);

		// Get transforms and update all the objects
		for (unsigned int i = 0; i < objects.size(); ++i)
		{
			glm::mat4 updateTransform;
			engine.getOpenGLMatrix(i, updateTransform);
			objects[i]->setTransform(updateTransform);
		}

		// Set the camera position by applying a "view matrix"
		// (as part of the model-view-projection graphics scheme)
		shader1.enable();
		vMatrix = glm::lookAt(
			glm::vec3(camx, camy, camz), // Camera position
			objects[0]->getWorldOrigin(), // Look at the ball
			glm::vec3(0.0f, 1.0f, 0.0f)  // Y-axis is Up
		);
		shader1.setUniformMat4("vvmat", vMatrix); // "Vertexshader-View-MATrix"
		shader1.disable();

		// Draw layers (and their objects) here
		stageLayer.render(glm::vec3(camx, camy, camz));
		modelLayer.render(glm::vec3(camx, camy, camz));

		if (cfg.raytrace)
			raytracer.render({});

		// Draw text last so it is on top of the other layers
		tw.begin();
		tw.write(perf.getFramesStr(),  -0.99f, +0.95f, sx, sy, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		tw.write(perf.getElapsedStr(), -0.99f, +0.90f, sx, sy, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		tw.write(perf.getFpsStr(),     -0.99f, +0.85f, sx, sy, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		tw.end();

		// Finally, update the window buffers to see what we've drawn
		window.update();

		perf.incrementFrames();
	}

	for (unsigned int i = 0; i < objects.size(); ++i)
	{
		delete objects[i];
	}

	return (0);
}

