/*
  cidtoacid.c -- convert CID files to ACID files (main routine)
  Copyright (C) 1999 Dieter Baron

  This file is part of ttftot42, to use TrueType fonts in PostScript.
  The author can be contacted at <dillo@giga.or.at>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/



#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>

#include "cid.h"
#include "substext.h"

#include "config.h"

char *prg;

char version_string[] = 
"cidtoacid (" PACKAGE " " VERSION ")\n\
Copyright (C) 1999 Dieter Baron\n"
PACKAGE " comes with ABSOLUTELY NO WARRANTY, to the extent permitted by law.\n\
You may redistribute copies of " PACKAGE "\n\
under the terms of the GNU General Public License.\n\
For more information about these matters, see the files named COPYING.\n";

char usage_string[] = "\
Usage: %s [-hV] [-o file] acid-file ...\n";

char help_string[] = "\
\n\
  -h, --help            display this help message\n\
  -V, --version         display version number\n\
\n\
  -o, --output FILE     output to FILE\n\
\n\
Report bugs to <dillo@giga.or.at>.\n";

#define OPTIONS	"hVo:"

struct option options[] = {
    { "help",      0, 0, 'h' },
    { "version",   0, 0, 'V' },
    { "output",    1, 0, 'o' },
    { NULL,        0, 0, 0   }
};



int
main(int argc, char **argv)
{
    struct cid *cid;
    char *outfile, *thisout;
    int ret, c;

    prg = argv[0];

    outfile = NULL;
    
    opterr = 0;
    while ((c=getopt_long(argc, argv, OPTIONS, options, 0)) != EOF) {
	switch (c) {
	case 'o':
	    outfile = optarg;
	    break;

	case 'h':
	    printf(usage_string, prg);
	    fputs(help_string, stdout);
	    exit(0);
	case 'V':
	    fputs(version_string, stdout);
	    exit(0);
	default:
	    fprintf(stderr, usage_string, prg);
	    exit(1);
	}
    }

    if (optind == argc || (outfile && optind != argc-1)) {
	fprintf(stderr, usage_string, prg);
	exit(1);
    }

    for (; optind < argc; optind++) {
	if ((cid=cid_read(argv[optind])) == NULL) {
	    /* XXX: should error output be done in cid_read? */
	    fprintf(stderr, "%s: can't read CID file `%s': %s\n",
		    prg, argv[optind], strerror(errno));
	    ret = 1;
	    continue;
	}
	if (!outfile)
	    thisout = substext(basename(argv[optind]), ".cid", ".acid");
	else
	    thisout = outfile;

	if (cid_awrite(thisout, cid))
	    ret = 1;

	cid_free(cid);
    }
    
    exit(ret);
}
