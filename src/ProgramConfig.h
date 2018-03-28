#ifndef ECE_6122_FINAL_PROJECT_CFG_H
#define ECE_6122_FINAL_PROJECT_CFG_H

#include <getopt.h>
#include <cstring>

struct ProgramConfig
{
	ProgramConfig() : verbose(0)
	{
		memset(modules_dir, 0, sizeof(modules_dir));
	}

	int verbose;
	char modules_dir[128];
};

#endif //ECE_6122_FINAL_PROJECT_CFG_H
