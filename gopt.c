/*      gopt.c
 *
 *  Copyright 2017 Robert L (Bob) Parker rlp1938@gmail.com
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA 02110-1301, USA.
*/

#include "fileops.h"
#include "stringops.h"
#include "gopt.h"

/* options are -h; --help, -n; --new and -a; --add */

options_t process_options(int argc, char **argv)
{
	synopsis =
  "\tSYNOPSIS\n"
  "\thtmake, generates and adds to makefile for markdown to make html\n\n"
  "\t -n, --new html_filename\n"
  "\t -a, --add target_name1, ... target_nameN\n"
  ;
	helptext =
  "\tOPTIONS\n"
  "\t-h, --help\n\n"
  "\tOutputs this help message and then quits.\n"
  "\t-n, --new target_name (usually index)\n"
  "\tCreates a new makefile to make target_name.\n"
  "\tError if makefile exists.\n"
  "\t-a, --add target_name\n"
  "\tAdds addtional target_name to makefile.\n"
  "\tError if makefile does not exist.\n"
  "\tRepeat use of this option for each additional target_name.\n\n"
  ;

	optstring = ":hn:a:";	// initialise

	/* declare and set defaults for local variables. */

	/* set up defaults for opt vars. */
	options_t opts = {0};	// assumes defaults all 0/NULL

	int c;

	while(1) {
		int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		static struct option long_options[] = {
		{"help",		0,	0,	'h' },
		{"add",		1,	0,	'a' },
		{"new",		1,	0,	'n' },
		{0,	0,	0,	0 }
		};

		c = getopt_long(argc, argv, optstring,
                        long_options, &option_index);

		if (c == -1)
			break;

		switch (c) {
		case 0:
			switch (option_index) {
			} // switch()
		break;
		case 'h':
		dohelp(0);
		break;
		case 'a':	// add target_names to makefile
		break;
		case 'n':	// create makefile with named target.
		if (!opts.newfile) {
			opts.newfile = 1;
			opts.newfilename = strdup(optarg);
		}	// just ignore subsequent invocations of this option.
		break;
		case ':':
			fprintf(stderr, "Option %s requires an argument\n",
					argv[this_option_optind]);
			dohelp(1);
		break;
		case '?':
			fprintf(stderr, "Unknown option: %s\n",
					 argv[this_option_optind]);
			dohelp(1);
		break;
		} // switch()
	} // while()
	return opts;
} // process_options()

void dohelp(int forced)
{
  if(strlen(synopsis)) fputs(synopsis, stderr);
  fputs(helptext, stderr);
  exit(forced);
} // dohelp()

void addons(options_t *opts, char *optarg)
{	/* malloc/realloc to process added target name */
	if (opts->addcount == 0) {
		opts->addcount = 1;
		opts->additions = calloc(2, sizeof(char *));
		if (!opts->additions) goto errexit;
		opts->additions[0] = strdup(optarg);
	} else {
		opts->additions[opts->addcount] = strdup(optarg);
			// was (char *)NULL
		opts->additions = realloc(opts->additions,
							(opts->addcount + 2) *sizeof(char *));
		if (!opts->additions) goto errexit;
		opts->addcount++;
		opts->additions[opts->addcount] = (char *)NULL;
	}
	return;
errexit:
	fputs("Memory allocation failure in addons().\n", stderr);
	exit(EXIT_FAILURE);
} // addons()
