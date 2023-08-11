/*
 * Copyright (C) 2023  Trevor Woerner <twoerner@gmail.com>
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>
#include "config.h"

static char *inputFileName_pG = NULL;
static char buf[256];

static int process_cmdline_args (int argc, char *argv[]);
static void usage (char *cmdline_p);
static void version (void);

int
main (int argc, char *argv[])
{
	int ret;
	FILE *inputFile_p;

	ret = process_cmdline_args(argc, argv);
	if (ret != 0)
		return 1;

	if (inputFileName_pG == NULL)
		inputFile_p = stdin;
	else {
		inputFile_p = fopen(inputFileName_pG, "r");
		if (inputFile_p == NULL) {
			perror("fopen()");
			return 1;
		}
	}

	while ((fgets(buf, sizeof(buf)-1, inputFile_p)) != NULL) {
		// if a line starts with '+CONFIG_*' output the line without
		// the leading '+'
		if (strncmp(buf, "+CONFIG", strlen("+CONFIG")) == 0)
			printf("%s", &buf[1]);

		// if a line starts with '+# CONFIG_* is not set' output the
		// line as: 'CONFIG_*=n'
		if (strncmp(buf, "+# CONFIG", strlen("+# CONFIG")) == 0) {
			// put a NULL after the CONFIG_* value
			char *ptr = &buf[3];
			while (*ptr != ' ')
				++ptr;
			*ptr = 0;
			printf("%s=n\n", &buf[3]);
		}

		// ignore all other lines
	}

	fclose(inputFile_p);
	return 0;
}

static void
version(void)
{
	printf("%s v%s\n", PACKAGE, PACKAGE_VERSION);
}

static void
usage(char *cmdline_p)
{
	printf("usage: %s [options]\n", cmdline_p);
	printf("operation:\n");
	printf("  The program expects to be fed a unified diff of 2 kconfigs.\n");
	printf("  By default it reads from stdin, but the --infile cmdline option\n");
	printf("  can be used to read the diff from a file instead.\n");
	printf("options:\n");
	printf("  -h|--help           Print this help and exit successfully.\n");
	printf("  -v|--version        Print the package version and exit successfully.\n");
	printf("  -i|--infile <file>  Take input from <file> instead of stdin.\n");
}

static int
process_cmdline_args(int argc, char *argv[])
{
	int c;
	struct option longOpts[] = {
		{"help", no_argument, NULL, 'h'},
		{"version", no_argument, NULL, 'v'},
		{"infile", required_argument, NULL, 'i'},
		{NULL, 0, NULL, 0},
	};

	while (1) {
		c = getopt_long(argc, argv, "hvi:", longOpts, NULL);
		if (c == -1)
			break;
		switch (c) {
			case 'h':
				version();
				usage(argv[0]);
				exit(0);
				break;

			case 'v':
				version();
				exit(0);
				break;

			case 'i':
				if (inputFileName_pG != NULL) {
					printf("infile can only be specified once\n");
					usage(argv[0]);
					exit(1);
				}
				inputFileName_pG = optarg;
				break;

			case '?':
				exit(1);
				break;

			default:
				break;
		}
	}

	if (optind < argc) {
		printf("extra cmdline args found\n");
		usage(argv[0]);
		exit(1);
	}

	return 0;
}
