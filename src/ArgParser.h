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
				{ "raytrace", no_argument, &cfg->raytrace, 'r' },
				{ "threads", required_argument, 0, 't' },
				{ "verbose", no_argument, &cfg->verbose, 'v' },
				{ "help", no_argument, 0, HELP_ARG },
				{ 0, 0, 0, 0 }
			};

			int i = 0;

			c = getopt_long(argc, argv, "rt:v?", long_opts, &i);

			if (c == -1)
				break;

			switch (c)
			{
				case 't': // Number of threads to use
					cfg->num_threads = strtod(optarg, NULL);
					break;
				case '?':
				case HELP_ARG:
					printf("Usage: %s [OPTIONS...]\n", argv[0]);
					printf("\n");
					printf("Options\n");
					printf("  -r, --raytrace            Enable raytracing\n");
					printf("  -t, --threads <NUM>       Number of threads to use\n");
					printf("  -v, --verbose             Print more messages\n");
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
