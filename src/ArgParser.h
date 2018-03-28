#ifndef ECE_6122_FINAL_PROJECT_ARGPARSER_H
#define ECE_6122_FINAL_PROJECT_ARGPARSER_H

#include "ProgramConfig.h"
#include <getopt.h>
#include <cstdio>

#define HELP_ARG  1000

class ArgParser
{
public:

	static void parse_args(int argc, char **argv, ProgramConfig& cfg)
	{
		int c;

		while (1)
		{
			static struct option long_opts[] =
			{
				{ "models-dir", required_argument, 0, 'd' },
				{ "verbose", no_argument, &cfg.verbose, 1 },
				{ "help", no_argument, 0, HELP_ARG },
				{ 0, 0, 0, 0 }
			};

			int i = 0;

			c = getopt_long(argc, argv, "d:hv", long_opts, &i);

			if (c == -1)
				break;

			switch (c)
			{
				case 'd': // modules directory
					strncpy(cfg.modules_dir, optarg, sizeof(cfg.modules_dir)-1);
					break;
				case 'v': // verbose flag
					cfg.verbose = 1;
					break;
				case '?':
				case HELP_ARG:
					printf("Print this stuff when the -?|--help arg shows up\n");
					break;
				default: // unknown argument, ignore
					break;
			}
		}
	}
};


#endif //ECE_6122_FINAL_PROJECT_ARGPARSER_H
