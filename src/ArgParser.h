#ifndef ECE_6122_FINAL_PROJECT_ARGPARSER_H
#define ECE_6122_FINAL_PROJECT_ARGPARSER_H

#include "ProgramConfig.h"
#include <getopt.h> // getopt_long()
#include <cstdio> // strncpy()
#include <cstdlib>

#define HELP_ARG  1000

/**
 * A very simple class to provide command-line parsing convenience.
 */
class ArgParser
{
public:

	/**
	 * Parses the command-line options given in argv and populates the
	 * @c ProgramConfig struct
	 * @param argc The number of command-line arguments
	 * @param argv A string array of the command-line arguments
	 * @param cfg The ProgramConfig struct to populate. Execution returns
	 * immediately if this is NULL.
	 */
	static void parse_args(int argc, char **argv, ProgramConfig *cfg)
	{
		int c;

		if (cfg == NULL)
			return;

		while (1)
		{
			static struct option long_opts[] =
			{
				{ "frames", required_argument, 0, 'f' },
				{ "raytrace", no_argument, &cfg->raytrace, 'r' },
				{ "scene", required_argument, 0, 's' },
				{ "threads", required_argument, 0, 't' },
				{ "verbose", no_argument, &cfg->verbose, 'v' },
				{ "help", no_argument, 0, HELP_ARG },
				{ 0, 0, 0, 0 }
			};

			int i = 0;

			c = getopt_long(argc, argv, "f:rs:t:v?", long_opts, &i);

			if (c == -1)
				break;

			switch (c)
			{
				case 'f': // Simulation time in seconds
					cfg->frames = strtoul(optarg, NULL, 10);
					break;
				case 'r': // Enable raytracing
					cfg->raytrace = 1;
					break;
				case 's': // Scene
					cfg->scene = strtoul(optarg, NULL, 10);
					if (cfg->scene < 1 or cfg->scene > 2)
						cfg->scene = 1;
					break;
				case 't': // Number of threads to use
					cfg->numThreads = strtoul(optarg, NULL, 10);
					break;
				case 'v': // Print more messages
					cfg->verbose = 1;
					break;
				case '?':
				case HELP_ARG:
					printf("Usage: %s [OPTIONS...]\n", argv[0]);
					printf("\n");
					printf("Options\n");
					printf("  -f, --frames <NUM>        Total number of frames to simulate\n");
					printf("                            Default=unlimited for scene 1\n");
					printf("                            Default=300 for scene 2 (raytracing)\n");
					printf("  -t, --threads <NUM>       Number of threads to use. Default=1\n");
					printf("  -r, --raytrace            Enable raytracing. Only supported for scene 2\n");
					printf("  -s, --scene <1 or 2>      Scene to render.\n");
					printf("                            Only scene 2 supports raytracing.\n");
					printf("                            Default=1\n");
					printf("  -v, --verbose             Print debug messages\n");
					printf("  -h, --help                Print this help message and exit\n");
					printf("\n");
					printf("Report bugs to <rory.rudolph@gatech.edu>\n");
					exit(0);
					break;
				default: // Unknown argument, ignore
					break;
			}
		}
	}
};


#endif //ECE_6122_FINAL_PROJECT_ARGPARSER_H
