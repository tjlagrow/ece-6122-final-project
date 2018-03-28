#ifndef ECE_6122_FINAL_PROJECT_CFG_H
#define ECE_6122_FINAL_PROJECT_CFG_H

#include <cstring> // memset()

/**
 * This is a structure to holds all the various program configuration options.
 * This is passed around to the various modules of the code
 */
struct ProgramConfig
{
	ProgramConfig() : verbose(0)
	{
		memset(models_dir, 0, sizeof(models_dir));
	}

	int verbose; // If 1, means to print more messages
	char models_dir[128]; // Choose finite amount of memory for ease
};

#endif //ECE_6122_FINAL_PROJECT_CFG_H
