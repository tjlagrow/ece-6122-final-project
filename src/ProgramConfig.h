#ifndef ECE_6122_FINAL_PROJECT_CFG_H
#define ECE_6122_FINAL_PROJECT_CFG_H

#define DEFAULT_MOVIE_FILENAME  "movie.mpeg"

#include <cstring> // memset()
#include <cstdio> // snprintf()

/**
 * This is a structure to holds all the various program configuration options.
 * This is passed around to the various modules of the code
 */
struct ProgramConfig
{
	ProgramConfig() : raytrace(0), num_threads(1), verbose(0)
	{
		memset(output_filename, 0, sizeof(output_filename));
		snprintf(output_filename, strlen(DEFAULT_MOVIE_FILENAME), "%s", DEFAULT_MOVIE_FILENAME);
	}

	char output_filename[128];
	int raytrace;
	int num_threads;
	int verbose; // If 1, means to print more messages
};

#endif //ECE_6122_FINAL_PROJECT_CFG_H
