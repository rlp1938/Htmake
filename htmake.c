/*    htmake.c
 *
 * Copyright 2017 Robert L (Bob) Parker rlp1938@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <limits.h>
#include <libgen.h>

// delete below as needed'
#include "fileops.h"
#include "stringops.h"
#include "gopt.h"

enum typ(DIR, FIL, STR, OTHR);
static char *validatearg(char *arg, char *text, int typ);


int main(int argc, char **argv)
{
	options_t opts = process_options(argc, argv);

	// now process the non-option arguments
	char *arg1 = validatearg(argv[1], "Source dir|file", DIR|FIL);
	char *arg2 = validatearg(argv[1], "Dest dir|file", DIR|FIL);

	free(arg1);
	free(arg2);
	return 0;
}//main()

char *validatearg(char *arg, char *text, int typ)
{	/* add code as needed */
	if (!arg) {
		fprintf("No %s provided.\n", text);
		exit(EXIT_FAILURE);
	}
	switch (typ)
	{
		case DIR:
			if (direxists(arg) == -1) {
				fprintf(stderr, "Does not existor or not a dir: %s\n",
								arg);
				exit(EXIT_FAILURE);
			}
			break;
		case FIL:
			if (fileexists(arg) == -1) {
				fprintf(stderr, "Does not exist or or not a file: %s\n",
								arg);
				exit(EXIT_FAILURE);
			}
			break;
		case STR:
			if (strlen(arg) == 0) {
				fprintf(stderr, "Zero length string provided.\n");
				exit(EXIT_FAILURE);
			}
			break;
		case OTHR:
			// Custom code here
			break;
	} // switch(typ)
	return dostrdup(arg);
} // validatearg()
