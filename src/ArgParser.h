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
				{ "dir", required_argument, 0, 'd' },
				{ "verbose", no_argument, &cfg->verbose, 1 },
				{ "help", no_argument, 0, HELP_ARG },
				{ 0, 0, 0, 0 }
			};

			int i = 0;

			c = getopt_long(argc, argv, "d:hv", long_opts, &i);

			if (c == -1)
				break;

			switch (c)
			{
				case 'd': // Models directory
					strncpy(cfg->models_dir, optarg, sizeof(cfg->models_dir)-1);
					break;
				case 'v': // Verbose flag
					cfg->verbose = 1;
					break;
				case '?':
				case HELP_ARG:
					printf("Usage: %s [-d|--dir <MODELS DIR>] [-v|--verbose] [-h|--help]\n", argv[0]);
					printf("\n");
					printf("Options\n");
					printf("  -d, --dir  <MODELS_DIR>   The directory of .obj and .mtl model files to load\n");
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
