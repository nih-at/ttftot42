#ifndef _HAD_T42_H
#define _HAD_T42_H

/*
  t42.h -- general header file
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

#include "config.h"

#ifdef HAVE_FREETYPE_FREETYPE_H
#include <freetype/freetype.h>
#else
#include <freetype.h>
#endif

#include "cid.h"

#ifdef HAVE_TT_INIT_GSUB_EXTENSION
#define HAVE_GSUB
#ifdef HAVE_FREETYPE_FREETYPE_H
#include <freetype/ftxopen.h>
#else
#include <ftxopen.h>
#endif
#endif /* gsub */

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
#define NTABLES     9        /* number of tables in table dir */
#define HEADER_LEN  (12+NTABLES*16)
                             /* length of header and table directory */
#define TABLE_GLYF  2        /* index of glyf table */
#define TABLE_HEAD  3        /* index of head table */
#define TABLE_LOCA  6        /* index of loca table */

/* encoding flags */
#define ENC_BUILTIN	0x1	/* built into PostScript interpreter */
#define ENC_FROMFONT	0x2	/* encoding vector is taken from font */



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

    struct table dir[NTABLES];

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
    int flags;
    char **vector;
    struct rev_enc *reverse;
    int nreverse;
};



extern char *prg;
extern TT_Engine fte;
extern struct encoding encoding[];
extern int nencoding;



int cid_mkmap(struct font *f, struct cid *cid, unsigned short **mapp);
void clear_encoding(struct encoding *enc);
void close_font(font *f);
int done(void);
int get_encoding(font *f, struct encoding *enc);
char *get_name(TT_Face f, int nnames, int name);
int init(void);
font *open_font(char *fname, int what, int type);
int write_afm(font *f, FILE *fout, struct encoding *encoding);
int write_cid2(font *f, FILE *fout, struct cid *cid);
int write_cidmap(font *f, struct cid *cid, FILE *fout);
int write_font_info(FILE *fout, font *f);
int write_sfnts(font *f, FILE *fout);
int write_t42(font *f, FILE *fout, struct encoding *encoding);
void *xmalloc(size_t size);
void *xrealloc(void *data, size_t size);

#endif /* t42.h */
