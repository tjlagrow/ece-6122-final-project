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

#define VERTEX_SHADER_PATH    "../shaders/vertex.glsl"
#define FRAGMENT_SHADER_PATH  "../shaders/fragment.glsl"

#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   600
#define WINDOW_TITLE    "ECE6122 Final Project"

static float fov = 45.0f; // Filed of View in degrees
static float camx = 0.01f; // Camera location x
static float camy = 20.0f; // Camera location y, this is the height
static float camz = 0.01f; // Camera location z

int verbose; // Global verbose

/**
 * TODO Abstract into a Scene class
 */
enum class ShapeType { Box, Sphere, Cylinder, Cone, Custom };

/**
 * TODO Abstract into a Scene class
 */
struct SceneObject
{
	ShapeType type;
	glm::vec3 position;
	float mass;
	float bounciness;
	float friction;
	glm::vec3 velocity;
	const char *filepath;
};

/**
 * Scene 1 objects
 * TODO Abstract into a Scene class
 */
static std::vector<struct SceneObject> scene1 =
{
	{ ShapeType::Sphere,   glm::vec3(-7.0f, +30.0f, +0.0f), 10.f, 1.0f, 0.9f, glm::vec3(), "../models/ball_simple.obj" },
	{ ShapeType::Custom,   glm::vec3(-8.0f, +00.0f, +0.0f), 99.f, 0.6f, 0.9f, glm::vec3(), "../models/ramp.obj" },
	{ ShapeType::Box,      glm::vec3(+6.0f, +02.0f, -1.0f), 0.4f, 0.6f, 0.9f, glm::vec3(), "../models/cube.obj" },
	{ ShapeType::Box,      glm::vec3(+6.0f, +04.0f, -1.0f), 0.4f, 0.6f, 0.9f, glm::vec3(), "../models/cube.obj" },
	{ ShapeType::Box,      glm::vec3(+6.0f, +06.0f, -1.0f), 0.4f, 0.6f, 0.9f, glm::vec3(), "../models/cube.obj" },
	{ ShapeType::Box,      glm::vec3(+6.0f, +08.0f, -1.0f), 0.4f, 0.6f, 0.9f, glm::vec3(), "../models/cube.obj" },
	{ ShapeType::Box,      glm::vec3(+6.0f, +10.0f, -1.0f), 0.4f, 0.6f, 0.9f, glm::vec3(), "../models/cube.obj" },
	{ ShapeType::Box,      glm::vec3(+6.0f, +12.0f, -1.0f), 0.4f, 0.6f, 0.9f, glm::vec3(), "../models/cube.obj" },
	{ ShapeType::Box,      glm::vec3(+6.0f, +02.0f, +1.0f), 0.4f, 0.6f, 0.9f, glm::vec3(), "../models/cube.obj" },
	{ ShapeType::Box,      glm::vec3(+6.0f, +04.0f, +1.0f), 0.4f, 0.6f, 0.9f, glm::vec3(), "../models/cube.obj" },
	{ ShapeType::Box,      glm::vec3(+6.0f, +06.0f, +1.0f), 0.4f, 0.6f, 0.9f, glm::vec3(), "../models/cube.obj" },
	{ ShapeType::Box,      glm::vec3(+6.0f, +08.0f, +1.0f), 0.4f, 0.6f, 0.9f, glm::vec3(), "../models/cube.obj" },
	{ ShapeType::Box,      glm::vec3(+6.0f, +10.0f, +1.0f), 0.4f, 0.6f, 0.9f, glm::vec3(), "../models/cube.obj" },
	{ ShapeType::Box,      glm::vec3(+6.0f, +12.0f, +1.0f), 0.4f, 0.6f, 0.9f, glm::vec3(), "../models/cube.obj" },
};

/**
 * Scene 2 objects
 * TODO Abstract into a Scene class
 */
static std::vector<struct SceneObject> scene2 =
{
	{ ShapeType::Sphere, glm::vec3(), 1.0f, 0.9f, 0.6f, glm::vec3(), "../models/ballgreen.obj" },
	{ ShapeType::Sphere, glm::vec3(), 1.0f, 0.9f, 0.6f, glm::vec3(), "../models/ballred.obj" },
	{ ShapeType::Sphere, glm::vec3(), 1.0f, 0.9f, 0.6f, glm::vec3(), "../models/ballblue.obj" },
	{ ShapeType::Sphere, glm::vec3(), 1.0f, 0.9f, 0.6f, glm::vec3(), "../models/ballyellow.obj" },
	{ ShapeType::Sphere, glm::vec3(), 1.0f, 0.9f, 0.6f, glm::vec3(), "../models/ballpink.obj" },
	{ ShapeType::Sphere, glm::vec3(), 1.0f, 0.9f, 0.6f, glm::vec3(), "../models/ballpurple.obj" },
	{ ShapeType::Sphere, glm::vec3(), 1.0f, 0.9f, 0.6f, glm::vec3(), "../models/ballcyan.obj" },
	{ ShapeType::Sphere, glm::vec3(), 1.0f, 0.9f, 0.6f, glm::vec3(), "../models/ballgreen.obj" },
	{ ShapeType::Sphere, glm::vec3(), 1.0f, 0.9f, 0.6f, glm::vec3(), "../models/ballred.obj" },
	{ ShapeType::Sphere, glm::vec3(), 1.0f, 0.9f, 0.6f, glm::vec3(), "../models/ballblue.obj" },
	{ ShapeType::Sphere, glm::vec3(), 1.0f, 0.9f, 0.6f, glm::vec3(), "../models/ballyellow.obj" },
	{ ShapeType::Sphere, glm::vec3(), 1.0f, 0.9f, 0.6f, glm::vec3(), "../models/ballpink.obj" },
	{ ShapeType::Sphere, glm::vec3(), 1.0f, 0.9f, 0.6f, glm::vec3(), "../models/ballpurple.obj" },
	{ ShapeType::Sphere, glm::vec3(), 1.0f, 0.9f, 0.6f, glm::vec3(), "../models/ballcyan.obj" },
	{ ShapeType::Sphere, glm::vec3(), 1.0f, 0.9f, 0.6f, glm::vec3(), "../models/ballgreen.obj" },
	{ ShapeType::Sphere, glm::vec3(), 1.0f, 0.9f, 0.6f, glm::vec3(), "../models/ballred.obj" },
	{ ShapeType::Sphere, glm::vec3(), 1.0f, 0.9f, 0.6f, glm::vec3(), "../models/ballblue.obj" },
	{ ShapeType::Sphere, glm::vec3(), 1.0f, 0.9f, 0.6f, glm::vec3(), "../models/ballyellow.obj" },
	{ ShapeType::Sphere, glm::vec3(), 1.0f, 0.9f, 0.6f, glm::vec3(), "../models/ballpink.obj" },
	{ ShapeType::Sphere, glm::vec3(), 1.0f, 0.9f, 0.6f, glm::vec3(), "../models/ballpurple.obj" },
	{ ShapeType::Sphere, glm::vec3(), 1.0f, 0.9f, 0.6f, glm::vec3(), "../models/ballcyan.obj" },
	{ ShapeType::Sphere, glm::vec3(), 1.0f, 0.9f, 0.6f, glm::vec3(), "../models/ballgreen.obj" },
	{ ShapeType::Sphere, glm::vec3(), 1.0f, 0.9f, 0.6f, glm::vec3(), "../models/ballred.obj" },
	{ ShapeType::Sphere, glm::vec3(), 1.0f, 0.9f, 0.6f, glm::vec3(), "../models/ballblue.obj" },
	{ ShapeType::Sphere, glm::vec3(), 1.0f, 0.9f, 0.6f, glm::vec3(), "../models/ballyellow.obj" },
	{ ShapeType::Sphere, glm::vec3(), 1.0f, 0.9f, 0.6f, glm::vec3(), "../models/ballpink.obj" },
	{ ShapeType::Sphere, glm::vec3(), 1.0f, 0.9f, 0.6f, glm::vec3(), "../models/ballpurple.obj" },
	{ ShapeType::Sphere, glm::vec3(), 1.0f, 0.9f, 0.6f, glm::vec3(), "../models/ballcyan.obj" }
};

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
	verbose = cfg.verbose;
	std::vector<SceneObject> *objptr;

	std::srand(std::time(0));

	if (cfg.verbose)
	{
		int pad = -15;
		printf("%*s: %s\n", pad, "Output file", cfg.output_filename);
		printf("%*s: %u\n", pad, "Scene", cfg.scene);
		printf("%*s: %s\n", pad, "Raytrace", cfg.raytrace ? "yes" : "no");
		printf("%*s: %d\n", pad, "Threads", cfg.numThreads);
		printf("%*s: %d\n", pad, "Frames", cfg.frames);
		printf("%*s: %s\n", pad, "Verbose", cfg.verbose ? "yes" : "no");
	}

	Window window(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);

	// The text writer is used to write text onto the window
	// It has no effect on the raytracing algorithm
	TextWriter tw(VS_FONT_PATH, FS_FONT_PATH, TTF_PATH, 16);

	// Initialize the raytracer
	Raytracer raytracer(window.getWidth(), window.getHeight(), fov, 4);

	Shader shader1(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

	Layer stageLayer(&shader1);
	Layer modelLayer(&shader1);

	Object stage("../models/stage.obj");
	stageLayer.add(&stage);

	// The physics engine is responsible for collision detection
	// and updating the objects' vertices each frame
	PhysicsEngine engine;

	switch (cfg.scene)
	{
		case 1:
			objptr = &scene1;
			cfg.raytrace = 0;
			cfg.frames = 0;
			camx = -15.0f; // Camera location x
			camy = 5.0f; // Camera location y, this is the height
			camz = 20.0f; // Camera location z
			engine.addWalls(1);
			break;
		case 2:
			objptr = &scene2;
			engine.addWalls(0);
			break;
		default:
			printf("Invalid scene selected\n");
			exit(0);
	}


	// Load the objects from file into the layer
	std::vector<Object *> objects;
	for (unsigned int i = 0; i < (*objptr).size(); ++i)
	{
		auto *o = new Object((*objptr)[i].filepath);
		glm::mat4 transform = glm::translate((*objptr)[i].position);
		o->setTransform(transform);
		modelLayer.add(o);
		objects.push_back(o);
		switch ((*objptr)[i].type)
		{
			case ShapeType::Sphere:
				if (cfg.scene == 2)
				{
					float multiplier;

					multiplier = (std::rand() % 2 == 1) ? -1 : 1;
					(*objptr)[i].velocity.x = std::rand() % 4 * multiplier;

					multiplier = (std::rand() % 2 == 1) ? -1 : 1;
					(*objptr)[i].velocity.z = std::rand() % 4 * multiplier;

					multiplier = (std::rand() % 2 == 1) ? -1 : 1;
					(*objptr)[i].position.x = std::rand() % 4 * multiplier;

					// No multiplier, always start above ground
					(*objptr)[i].position.y = std::rand() % 40;

					multiplier = (std::rand() % 2 == 1) ? -1 : 1;
					(*objptr)[i].position.z = std::rand() % 4 * multiplier;
				}
				engine.addSphere(
					1.0f,
					(*objptr)[i].mass,
					(*objptr)[i].bounciness,
					(*objptr)[i].friction,
					(*objptr)[i].velocity,
					(*objptr)[i].position
				);
				break;
			case ShapeType::Box:
				engine.addBox(
					o->getSize()/2.0f, // Bullet wants half-lengths
					(*objptr)[i].mass,
					(*objptr)[i].bounciness,
					(*objptr)[i].friction,
					(*objptr)[i].velocity,
					(*objptr)[i].position
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
					(*objptr)[i].mass,
					(*objptr)[i].bounciness,
					(*objptr)[i].friction,
					(*objptr)[i].velocity,
					(*objptr)[i].position
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
		if (perf.getElapsedTime() > 1.0f)
		{
			perf.updateStats(); // Update the performance statistics
			perf.reset();
		}

		engine.stepSimulation(1 / 60.0f); // Speed up to like 1/24 if too slow
		std::vector<glm::vec3> positions;
		engine.getMotionStates(positions);

		// Get transforms and tell OpenGL about new object positions
		for (unsigned int i = 0; i < objects.size(); ++i)
		{
			glm::mat4 updateTransform;
			engine.getOpenGLMatrix(i, updateTransform);
			objects[i]->setTransform(updateTransform);
		}

		// Set the camera position by applying a "view matrix"
		// (as part of the model-view-projection graphics scheme)
		shader1.enable();
		glm::vec3 lookAt;
		switch (cfg.scene)
		{
			case 1:
				lookAt = objects[0]->getWorldOrigin(); // Look at the ball
				break;
			case 2:
				lookAt = glm::vec3();
				break;
		}
		vMatrix = glm::lookAt(
			glm::vec3(camx, camy, camz), // Camera position
			lookAt,
			glm::vec3(0.0f, 1.0f, 0.0f)  // Y-axis is Up
		);
		shader1.setUniformMat4("vvmat", vMatrix); // "Vertexshader-View-MATrix"
		shader1.disable();

		// Draw layers (and their objects) here
		stageLayer.render(glm::vec3(camx, camy, camz));
		modelLayer.render(glm::vec3(camx, camy, camz));

		if (cfg.raytrace)
		{
			raytracer.render(
				glm::vec3(camx, camy, camz), // Camera position
				glm::vec3(0.0f, 0.0f, 0.0f), // Look at
				objects
			);
		}

		// Draw text last so it is on top of the other layers
		tw.begin();
		tw.write(perf.getFramesStr(),  -0.99f, +0.95f, sx, sy, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		tw.write(perf.getElapsedStr(), -0.99f, +0.90f, sx, sy, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		tw.write(perf.getFpsStr(),     -0.99f, +0.85f, sx, sy, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		tw.end();

		// Finally, update the window buffers to see what we've drawn
		window.update();

		perf.incrementFrames();

		if (cfg.frames > 0 && perf.getFramesTotal() > cfg.frames)
			window.close();
	}

	if (cfg.raytrace)
	{
		if (system("ffmpeg -y -framerate 60 -i image%04d.ppm movie.mpeg"))
			printf("ERROR: Could not make movie. Run manually: 'ffmpeg -framerate 60 -i image%%04d.ppm movie.mpeg'\n");
	}

	for (unsigned int i = 0; i < objects.size(); ++i)
	{
		delete objects[i];
	}

	return (0);
}

