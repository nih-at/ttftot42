#ifndef _HAD_T42_H
#define _HAD_T42_H

/*
  t42.h -- general header file
  Copyright (C) 1998 Dieter Baron

  This file is part of otftot42, to use TrueType fonts in PostScript.
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

#define SCALE(fu)	((int)(((fu)*1000)/f->units_per_em))

#define TTAG_cvt   MAKE_TT_TAG( 'c', 'v', 't', ' ' )
#define TTAG_fpgm  MAKE_TT_TAG( 'f', 'p', 'g', 'm' )
#define TTAG_glyf  MAKE_TT_TAG( 'g', 'l', 'y', 'f' )
#define TTAG_head  MAKE_TT_TAG( 'h', 'e', 'a', 'd' )
#define TTAG_hhea  MAKE_TT_TAG( 'h', 'h', 'e', 'a' )
#define TTAG_hmtx  MAKE_TT_TAG( 'h', 'm', 't', 'x' )
#define TTAG_loca  MAKE_TT_TAG( 'l', 'o', 'c', 'a' )
#define TTAG_maxp  MAKE_TT_TAG( 'm', 'a', 'x', 'p' )
#define TTAG_prep  MAKE_TT_TAG( 'p', 'r', 'e', 'p' )



struct bbox {
    int llx, lly, urx, ury;
};

struct font {
    TT_Face face;

    int nglyph;
    int nnames;
    char *version;
    char *tt_version;
    int vm_min;
    int vm_max;
    int units_per_em;

    char *notice;
    char *full_name;
    char *family_name;
    char *weight;
    int is_fixed_pitch;
    char *italic_angle;
    int underline_position;
    int underline_thickness;

    struct bbox font_bbox;

    /* encoding_scheme */
    /* cap_height */
    /* x_height */
    /* ascender */
    /* descender */
    
    char *font_name;
};

typedef struct font font;

extern char *prg;
extern TT_Engine fte;
extern char *enc_standard[256];

int init(void);
font *open_font(char *fname);
void close_font(font *f);
char *get_name(TT_Face f, int nnames, int name);
int write_t42(font *f, FILE *fout);
int write_afm(font *f, FILE *fout);

#endif /* t42.h */
