#ifndef _HAD_T42_H
#define _HAD_T42_H

/*
  t42.h -- general header file
  Copyright (C) 1998, 1999 Dieter Baron

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
#include <freetype.h>

#include "config.h"

#ifdef HAVE_TT_INIT_GSUB_EXTENSION
#define HAVE_GSUB
#include <ftxopen.h>
#endif

#if (defined(HAVE_LIBGEN_H) && defined(HAVE_BASENAME))
#include <libgen.h>
#endif

#ifndef HAVE_BASENAME
char *basename(char *name);
#endif

#ifndef HAVE_STRDUP
char *strdup(char *name);
#endif


#define SCALE(fu)	((int)(((fu)*1000)/f->units_per_em))

#define WHAT_FONT  0x1   /* output font */
#define WHAT_AFM   0x2   /* output AFM file */
#define WHAT_NAME  0x4   /* print FontName to stdout */
#define WHAT_FILE  0x8   /* print FontName and file name to stdout */

#define TYPE_T42   0x0   /* Type 42 */
#define TYPE_CID   0x1   /* CID keyed font */
#define TYPE_PFB   0x2   /* Type 1 */

#define MAX_STRLEN  65534    /* max. PostScript string length - 1 */
#define LINE_LEN    36       /* length of one line (in bytes) */
#define HEADER_LEN  12+9*16  /* length of header and table directory */
#define TABLE_GLYF  2        /* index of glyf table */
#define TABLE_HEAD  3        /* index of head table */
#define TABLE_LOCA  6        /* index of loca table */



struct bbox {
    int llx, lly, urx, ury;
};

struct table {
    char tag[5];
    unsigned long checksum;
    unsigned long offset;
    unsigned long length;
};

struct font {
    TT_Face face;
    TT_Instance fi;
    TT_Glyph fg;
#ifdef HAVE_GSUB
    TTO_GSUBHeader *gsub;
#endif

    struct table dir[9];

    int nglyph;
    int nnames;
    int ncmaps;
    char *version;
    char *tt_version;
    int vm_min;
    int vm_max;
    int units_per_em;
    int long_loc;

    char *notice;
    char *full_name;
    char *family_name;
    char *weight;
    int is_fixed_pitch;
    char *italic_angle;
    int underline_position;
    int underline_thickness;
    int ascender;
    int descender;
    int capheight;
    int xheight;

    struct bbox font_bbox;

    /* encoding_scheme */
    /* cap_height */
    /* x_height */
    
    char *font_name;
};

typedef struct font font;

struct rev_enc {
    char *name;
    int code;
};

struct encoding {
    char *name, *full_name;
    int builtinp;
    char **vector;
    struct rev_enc *reverse;
    int nreverse;
};

struct cid {
    char *registry, *ordering;
    int supplement;
};



extern char *prg;
extern TT_Engine fte;
extern struct encoding encoding[];
extern int nencoding;



void *xmalloc(size_t size);

int init(void);
int done(void);
font *open_font(char *fname, int what, int type);
void close_font(font *f);
char *get_name(TT_Face f, int nnames, int name);
int write_t42(font *f, FILE *fout, struct encoding *encoding);
int write_cid2(font *f, FILE *fout, struct cid *cid);
int write_afm(font *f, FILE *fout, struct encoding *encoding);

int write_cidmap(font *f, struct cid *cid, FILE *fout);

/* util.h */
int write_font_info(FILE *fout, font *f);
int write_sfnts(font *f, FILE *fout);


#endif /* t42.h */
