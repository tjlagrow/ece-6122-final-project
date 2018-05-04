#ifndef ECE_6122_FINAL_PROJECT_CFG_H
#define ECE_6122_FINAL_PROJECT_CFG_H

#define DEFAULT_MOVIE_FILENAME  "movie.mpeg"
#define DEFAULT_SCENE 2
#define DEFAULT_FRAMES (60*2)
#define DEFAULT_NUM_THREADS  1

#include <cstring> // memset()
#include <cstdio> // snprintf()

/**
 * This is a structure to holds all the various program configuration options.
 * This is passed around to the various modules of the code
 */
struct ProgramConfig
{
	ProgramConfig()
	{
		raytrace = 0;
		scene = DEFAULT_SCENE;
		numThreads = DEFAULT_NUM_THREADS;
		frames = DEFAULT_FRAMES;
		verbose = 0;
		memset(output_filename, 0, sizeof(output_filename));
		snprintf(output_filename, sizeof(output_filename)-1, "%s", DEFAULT_MOVIE_FILENAME);
	}

	char output_filename[128];
	int raytrace;
	unsigned int scene;
	unsigned int numThreads;
	unsigned int frames;
	int verbose; // If 1, means to print more messages
};

#endif //ECE_6122_FINAL_PROJECT_CFG_H
