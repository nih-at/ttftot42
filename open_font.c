/*
  open_font -- open font file and extract needed information
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



#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include <ftxpost.h>
#include <ftxerr18.h>

#include "t42.h"

char *fixed2str(TT_Fixed f, int prec);



font *
open_font(char *fname)
{
    font *f;
    int err;

    TT_Face face;
    TT_Face_Properties prop;
    TT_Postscript *ps;
    TT_Header *hdr;
    TT_Post post;

    if ((f=(font *)malloc(sizeof(font))) == NULL)
	return NULL;

    if ((err=TT_Open_Face(fte, fname, &face)) != TT_Err_Ok) {
	fprintf(stderr, "%s: can't open font %s: %s\n",
		prg, fname, TT_ErrToString18(err));
	return NULL;
    }
    TT_Get_Face_Properties(face, &prop);
    TT_Load_PS_Names(face, &post);

    hdr = prop.header;
    ps = prop.postscript;
    
    /* fill out info */

    f->face = face;
    f->nglyph = prop.num_Glyphs;
    f->nnames = prop.num_Names;
    f->units_per_em = hdr->Units_Per_EM;
    
    f->font_bbox.llx = SCALE(hdr->xMin);
    f->font_bbox.lly = SCALE(hdr->yMin);
    f->font_bbox.urx = SCALE(hdr->xMax);
    f->font_bbox.ury = SCALE(hdr->yMax);

    f->vm_min = ps->minMemType42;
    f->vm_max = ps->maxMemType42;
    f->italic_angle = fixed2str(ps->italicAngle, 3);
    f->underline_position = ps->underlinePosition;
    f->underline_thickness = ps->underlineThickness;
    f->is_fixed_pitch = ps->isFixedPitch;

    f->tt_version = strdup("001.000"); /* otherwise freetype wont open it */
    f->version = (char *)malloc(8);
    sprintf(f->version, "%03ld.%03ld",
	    hdr->Font_Revision >> 16,
	    ((hdr->Font_Revision & 0xffff)*1000)/0x10000);

    f->font_name = get_name(f->face, f->nnames, TT_NAME_ID_PS_NAME);
    f->full_name = get_name(f->face, f->nnames, TT_NAME_ID_FULL_NAME);
    f->family_name = get_name(f->face, f->nnames, TT_NAME_ID_FONT_FAMILY);
    /* XXX: the following is wrong */
    f->weight = get_name(f->face, f->nnames, TT_NAME_ID_FONT_SUBFAMILY);
    f->notice = get_name(f->face, f->nnames, TT_NAME_ID_COPYRIGHT);

    return f;
}



char *
fixed2str(TT_Fixed f, int prec)
{
    char b[23], *p;
    int r, fr;
    
    p = b;
    if (prec == 0)
	prec = INT_MAX;

    sprintf(p, "%d", (int)f>>16);
    if ((f & 0xffff) != 0) {
	f &= 0xffff;
	p = p+strlen(p);
	*(p++) = '.';

	fr = 0;
	while (f != 0) {
	    f *= 10;
	    r = f/0x10000;
	    if (prec > 0)
		fr = fr*10 + r;
	    else {
		fr += r>=5;
		break;
	    }
	    f -= r*0x10000;
	    --prec;
	}
	sprintf(p, "%d", fr);
    }
    
    return strdup(b);
}
