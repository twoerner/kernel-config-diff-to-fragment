/*
 * Copyright (C) 2023  Trevor Woerner <twoerner@gmail.com>
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>

static char *inputFileName_pG = NULL;
static char buf[256];

static int process_cmdline_args (int argc, char *argv[]);
static void usage (char *cmdline_p);

int
main (int argc, char *argv[])
{
	int ret;
	FILE *inputFile_p;

	ret = process_cmdline_args(argc, argv);
	if (ret != 0)
		return 1;

	inputFile_p = fopen(inputFileName_pG, "r");
	if (inputFile_p == NULL) {
		perror("fopen()");
		return 1;
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
usage(char *cmdline_p)
{
	printf("usage: %s [options] <unified diff file>\n", cmdline_p);
	printf("options:\n");
	printf("  -h|--help        Print this help and exit successfully.\n");
}

static int
process_cmdline_args(int argc, char *argv[])
{
	int c;
	struct option longOpts[] = {
		{"help", no_argument, NULL, 'h'},
		{NULL, 0, NULL, 0},
	};

	while (1) {
		c = getopt_long(argc, argv, "h", longOpts, NULL);
		if (c == -1)
			break;
		switch (c) {
			case 'h':
				usage(argv[0]);
				exit(0);
				break;

			case '?':
				exit(1);
				break;

			default:
				break;
		}
	}

	if ((optind + 1) != argc) {
		printf("1 cmdline arg is required\n");
		usage(argv[0]);
		return -1;
	}

	inputFileName_pG = argv[1];
	return 0;
}
