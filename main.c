/*
  main -- main function
  Copyright (C) 1998, 1999 Dieter Baron

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
#include <unistd.h>
#include <errno.h>
#include <getopt.h>

#include "t42.h"
#include "substext.h"

char *prg;

char version_string[] = 
PACKAGE " " VERSION "\n\
Copyright (C) 1999 Dieter Baron\n"
PACKAGE " comes with ABSOLUTELY NO WARRANTY, to the extent permitted by law.\n";

char usage_string[] = "\
Usage: %s [-hVafnNcF] [-o file] [-e encoding] ttf-file ...\n";

char help_string[] = "\
\n\
  -h, --help            display this help message\n\
  -V, --version         display version number\n\
\n\
  -a, --afm             write afm file\n\
  -f, --font            write t42 file\n\
  -n, --name            print FontName to standard output\n\
  -N, --filename        print FontName and file name to standard output\n\
  -o, --output FILE     output to FILE\n\
  -c, --stdout          output to standard output\n\
  -e, --encoding ENC    encoding to use (std, pdf, latin1, 1252, font)\n\
  -C, --cid COLL        create CID keyed font (with collection COLL)\n\
  -F, --full            include full TrueType font file (not yet implemented)\n\
\n\
Report bugs to <dillo@giga.or.at>.\n";

#define OPTIONS	"hVafnNo:ce:C:F"

struct option options[] = {
    { "help",      0, 0, 'h' },
    { "version",   0, 0, 'V' },
    { "name",      0, 0, 'n' },
    { "afm",       0, 0, 'a' },
    { "font",      0, 0, 'f' },
    { "name",      0, 0, 'n' },
    { "filename",  0, 0, 'N' },
    { "output",    1, 0, 'o' },
    { "stdout",    0, 0, 'c' },
    { "encoding",  1, 0, 'e' },
    { "cid",       1, 0, 'C' },
    { "full",      0, 0, 'F' },
    { NULL,        0, 0, 0   }
};

struct cid *find_cid(char *name);



int
main(int argc, char **argv)
{
    extern int opterr, optind;
    extern char *optarg;
    struct encoding *enc;
    struct cid *cid;

    int err, i;
    int c, what, type, full, cat;
    char *outfile;
    font *f;
    FILE *fout;

    char *fontfile;

    prg = argv[0];

    cat = full = what = type = 0;
    enc = &encoding[0];
    outfile = NULL;

    opterr = 0;
    while ((c=getopt_long(argc, argv, OPTIONS, options, 0)) != EOF) {
	switch (c) {
	case 'f':
	    what |= WHAT_FONT;
	    break;
	case 'a':
	    what |= WHAT_AFM;
	    break;
	case 'n':
	    what |= WHAT_NAME;
	    break;
	case 'N':
	    what |= WHAT_FILE;
	    break;
	case 'F':
	    full = 1;
	    break;
	case 'e':
	    for (i=0; i<nencoding; i++) {
		if (strcmp(optarg, encoding[i].name) == 0) {
		    enc = &encoding[i];
		    break;
		}
	    }
	    if (i == nencoding) {
		fprintf(stderr, "%s: unknown encoding `%s'\n",
			prg, optarg);
		exit(1);
	    }
	    break;
	case 'C':
	    type |= TYPE_CID;
	    cid = find_cid(optarg);
	    if (cid == NULL) {
		fprintf(stderr, "%s: cid map `%s' not found\n",
			prg, optarg);
		exit(1);
	    }
	    break;
	case 'o':
	    outfile = optarg;
	    break;
	case 'c':
	    cat = 1;
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

    if (optind == argc) {
	fprintf(stderr, usage_string, prg);
	exit(1);
    }

    init();

    /* post process options */
    if (cat && outfile) {
	fprintf(stderr, "%s: can't write to both standard output "
		"and specified file\n",
		prg);
	exit(1);
    }
    if (cat || outfile) {
	if (optind+1 != argc) {
	    fprintf(stderr, "%s: can't write more than one font to %s\n",
		    prg, cat ? "standard output" : "specified file");
	    exit(1);
	}
	    
	switch (what) {
	case 0:
	    what = WHAT_FONT;
	    break;
	case WHAT_FONT|WHAT_AFM:
	    fprintf(stderr, "%s: can't write both font and afm to %s\n",
		    prg, cat ? "standard output" : "specified file");
	    exit(1);
	}
    }
    if ((what & (WHAT_NAME|WHAT_FILE)) == (WHAT_NAME|WHAT_FILE)) {
	fprintf(stderr, "%s: can't write both FontName and Fontmap line to "
		"standard output\n", prg);
	exit(1);
    }
    if (cat && what & (WHAT_NAME|WHAT_FILE)) {
	fprintf(stderr, "%s: can't write both font and %s to "
		"standard output\n",
		prg,
		(what & WHAT_NAME) ? "FontName" : "Fontmap line");
	exit(1);
    }
    if (what == 0)
	what = WHAT_FONT|WHAT_AFM;

    err = 0;
    for (; optind<argc; optind++) {
	fontfile = argv[optind];

	if ((f=open_font(fontfile, what, type)) == NULL) {
	    err = 1;
	    continue;
	}

	if ((enc->flags & ENC_FROMFONT) && (what & (WHAT_AFM|WHAT_FONT))) {
	    if (get_encoding(f, enc) < 0) {
		close_font(f);
		err = 1;
		continue;
	    }
	}

	if (what & WHAT_AFM) {
	    if (cat)
		fout = stdout;
	    else if (outfile) {
		if ((fout=fopen(outfile, "w")) == NULL) {
		    fprintf(stderr, "%s: can't create file `%s': %s\n",
			    prg, outfile, strerror(errno));
		    err = 1;
		}
	    }
	    else {
		outfile = substext(basename(fontfile), ".ttf", ".afm");
		if ((fout=fopen(outfile, "w")) == NULL) {
		    fprintf(stderr, "%s: can't create file `%s': %s\n",
			    prg, outfile, strerror(errno));
		    err = 1;
		}
		outfile = NULL;
	    }
	    if (fout) {
		write_afm(f, fout, enc);
		fclose(fout);
	    }
	}
	if (what & WHAT_FONT) {
	    if (cat)
		fout = stdout;
	    else if (outfile) {
		if ((fout=fopen(outfile, "w")) == NULL) {
		    fprintf(stderr, "%s: can't create file `%s': %s\n",
			    prg, outfile, strerror(errno));
		    err = 1;
		}
	    }
	    else {
		outfile = substext(basename(fontfile), ".ttf", ".t42");
		if ((fout=fopen(outfile, "w")) == NULL) {
		    fprintf(stderr, "%s: can't create file `%s': %s\n",
			    prg, outfile, strerror(errno));
		    err = 1;
		}
		outfile = NULL;
	    }
	    if (fout) {
		if (type == TYPE_T42)
		    write_t42(f, fout, enc);
		else
		    write_cid2(f, fout, cid);
		fclose(fout);
	    }
	}
	if (what & WHAT_NAME)
	    printf("%s\n", f->font_name);
	if (what & WHAT_FILE) {
#if 0
	    if (!outfile)
		outfile = substext(basename(fontfile), ".ttf", ".t42");
	    printf("/%s\t(%s) ;\n", f->font_name, outfile);
	    outfile = NULL;
#endif
	    printf("%s\t%s\n",
		   f->font_name,
		   outfile ? outfile : basename(fontfile));
	}

	clear_encoding(enc);
	close_font(f);
    }

    done();

    exit(err);
}



static char *cid_path[] = {
    DATADIR,
    NULL
};

struct cid *
find_cid(char *name)
{
    struct cid *cid;
    char b[8192], b2[8192], *registry, *ordering, *p;
    FILE *f;
    int i, supplement;

    if (strchr(name, '/'))
	return cid_read(name);

    if (strchr(name, '-')) {
	strcpy(b2, name);
	registry = strtok(b2, "-");
	ordering = strtok(NULL, "-");
	p = strtok(NULL, "-");
	if (p)
	    supplement = atoi(p);
	else
	    supplement = -1;
	
	for (i=0; cid_path[i]; i++) {
	    sprintf(b, "%s/index", cid_path[i]);
	    if ((f=fopen(b, "r")) == NULL)
		continue;
	    while (fgets(b, 8192, f)) {
		p = strtok(b, " \t\n");
		if (p && strcasecmp(registry, p) != 0)
		    continue;
		p = strtok(NULL, " \t\n");
		if (p && strcasecmp(ordering, p) != 0)
		    continue;
		p = strtok(NULL, " \t\n");
		p = strtok(NULL, " \t\n");
		if (p) {
		    sprintf(b2, "%s/%s.cid", cid_path[i], p);
		    cid = cid_read(b2);
		    if (cid != NULL) {
			fclose(f);
			if (supplement != -1) {
			    if (cid->nsupl > supplement)
				cid->supplement = supplement;
			}
			return cid;
		    }
		}
	    }
	}
    }

    for (i=0; cid_path[i]; i++) {
	sprintf(b, "%s/%s", cid_path[i], name);
	if ((cid=cid_read(b)))
	    return cid;
	sprintf(b, "%s/%s.cid", cid_path[i], name);
	if ((cid=cid_read(b)))
	    return cid;
    }

    return NULL;
}
