/*
  cidtoacid.c -- convert CID files to ACID files (main routine)
  Copyright (C) 1999 Dieter Baron

  This file is part of ttftot42, to use TrueType fonts in PostScript.
  The author can be contacted at <dillo@giga.or.at>

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:
  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.
  3. The name of the author may not be used to endorse or promote
     products derived from this software without specific prior
     written permission.
 
  THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS
  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
  IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
  IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>

#include "cid.h"
#include "substext.h"

#include "config.h"

char *prg;

char version_string[] = 
"cidtoacid (" PACKAGE " " VERSION ")\n\
Copyright (C) 1999 Dieter Baron\n"
PACKAGE " comes with ABSOLUTELY NO WARRANTY, to the extent permitted by law.\n";

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

    ret = 0;
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
