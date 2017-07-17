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

// delete below as needed.
#include "fileops.h"
#include "stringops.h"
#include "gopt.h"

static void writenew(char *newtarget);
static void inserttarget(char *target);
static char *buildmakelines(char *target);
static void writeadds(char **additions);

int main(int argc, char **argv)
{
	options_t opts = process_options(argc, argv);
	if (opts.newfile) {
		if (fileexists("makefile") != -1) {
			fputs("makefile exists already,"
			" new option not permitted.\n", stderr);
			exit(EXIT_FAILURE);
		}
		writenew(opts.newfilename);
	}
	// Ok to initialise the makefile and and the extras in 1 hit.
	if (opts.addcount) {
		if (fileexists("makefile") == -1) {
			fputs("makefile does not exist,"
			" add option not permitted.\n", stderr);
			exit(EXIT_FAILURE);
		}
		writeadds(opts.additions);
	}

	// now process the non-option arguments
	return 0;
}//main()

void writenew(char *newtarget)
{	/* create a makefile with a single target */
	FILE* fpo = dofopen("makefile", "w");
	fputs("# makefile for markdown => html\n", fpo);
	fprintf(fpo, "html :\n");
	fputs("# Files named *.o are not object files, they are html stubs,"
	" but the\n# naming will cause my cleanup scripts to zap them when "
	"run.\n\n",
	fpo);
	fputs("\nclean :\n\trm *.o\n", fpo);
	dofclose(fpo);
	sync();
	inserttarget(newtarget);
} // writenew()

static void inserttarget(char *target)
{	/* write the text for another target name into the makefile. */
	const int ll = 128;
	char line[ll];
	fdata myfd = readfile("makefile", 4096, 1);
	myfd.to -= 4096;	// want to keep track of the actual data only.
	// add to dependency list at top of file.
	char *insertpoint = myfd.from;
	//No failure guard here because the data has been program generated.
	insertpoint = memmem(insertpoint, myfd.to - insertpoint, "html :",
							strlen("html :"));
	insertpoint = memchr(insertpoint, '\n', myfd.to - insertpoint);
	// build the target name
	sprintf(line, " %s.html", target);
	size_t len = strlen(line);
	memmove(insertpoint + len, insertpoint, myfd.to - insertpoint);
	memcpy(insertpoint, line, len);
	myfd.to += len;
	// the make lines for the target name.
	char *what = buildmakelines(target);
	len = strlen(what);
	insertpoint = memmem(insertpoint, myfd.to - insertpoint, "clean :",
							strlen("clean :"));
	memmove(insertpoint + len, insertpoint, myfd.to - insertpoint);
	memcpy(insertpoint, what, len);
	myfd.to += len;
	writefile("makefile", myfd.from, myfd.to, "w");
} // inserttarget()

char *buildmakelines(char *target)
{	/* construct the make lines needed for target */
	const int ll = 128;
	char line[ll];
	static char ret[4096] = {0};
	// Clean out anything left around from any prior invocations.
	ret[0] = 0;
	// dependencies for target
	sprintf(line, "%s.html : %stop.o %smid.o %sbtm.o\n",
			target, target, target, target);
	strcat(ret, line);
	// make target
	sprintf(line, "\tcat %stop.o %smid.o %sbtm.o > %s.html\n\n",
			target, target, target, target);
	strcat(ret, line);
	// dependencies for header
	sprintf(line, "%stop.o : %stop.html\n", target, target);
	strcat(ret, line);
	// make header
	sprintf(line, "\tsed 's/filename/%s/' %stop.html > %stop.o\n\n",
				target, target, target);
	strcat(ret, line);
	// dependencies for middle
	sprintf(line, "%smid.o : %smid.md\n", target, target);
	strcat(ret, line);
	// make middle
	sprintf(line, "\tmarkdown %smid.md > %smid.o\n\n", target,
				target);
	strcat(ret, line);
	// dependencies for bottom
	sprintf(line, "%sbtm.o : %sbtm.html\n", target, target);
	strcat(ret, line);
	// make middle
	sprintf(line, "\tcp %sbtm.html %sbtm.o\n\n", target, target);
	strcat(ret, line);
	strcat(ret, "\n");
	return ret;
} // buildmakelines()

void writeadds(char **additions)
{	/* add a list of names to the make file */
	size_t idx = 0;
	while (additions[idx]) {
		inserttarget(additions[idx]);
		idx++;
	}
} // writeadds()
