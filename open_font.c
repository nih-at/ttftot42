/*
  open_font -- open font file and extract needed information
  Copyright (C) 1998 Dieter Baron

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



#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

#include <ftxpost.h>
#include <ftxerr18.h>

#include "t42.h"

static char *weight_name[] = {
    "Thin", "Extra Light", "Light", "Regular", "Medium",
    "Semibold", "Bold", "Extra Bold", "Black"
};
int nweight = sizeof(weight_name)/sizeof(weight_name[0]);

static char *fixed2str(TT_Fixed f, int prec);
static int read_dir(font *f, char *fname);



font *
open_font(char *fname, int what)
{
    font *f;
    int err, i, j, weight;
    char *version;

    TT_Face face;
    TT_Face_Properties prop;
    TT_Postscript *ps;
    TT_Header *hdr;
    TT_OS2 *os2;
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
    os2 = prop.os2;
    ps = prop.postscript;
    
    /* fill out info */

    f->face = face;
    f->nglyph = prop.num_Glyphs;
    f->nnames = prop.num_Names;
    f->long_loc = hdr->Index_To_Loc_Format;
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

    weight = (os2->usWeightClass-50)/100;
    if (weight >= nweight)
	weight = nweight-1;
    f->weight = strdup(weight_name[weight]);
    f->ascender = SCALE(os2->sTypoAscender);
    f->descender = SCALE(os2->sTypoDescender);

    f->tt_version = strdup("001.000"); /* FreeType only supports version 1.0 */
    f->version = (char *)malloc(8);
    version = get_name(f->face, f->nnames, TT_NAME_ID_VERSION_STRING);
    if ((version && strncasecmp(version, "Version ", 8) == 0
	 && isdigit(version[8]) && version[9] == '.'
	 && isdigit(version[10]) && isdigit(version[10]))) {
	sprintf(f->version, "00%c.%c%c0",
		version[8], version[10], version[11]);
    }
    else
	sprintf(f->version, "%03ld.%03ld",
		hdr->Font_Revision >> 16,
		((hdr->Font_Revision & 0xffff)*1000)/0x10000);
    
    f->font_name = get_name(f->face, f->nnames, TT_NAME_ID_PS_NAME);
    for (j=i=0; i<strlen(f->font_name); i++) {
	if (!strchr(" \t\n\r()<>[]/%", f->font_name[i]))
	    f->font_name[j++] = f->font_name[i];
    }
    f->full_name = get_name(f->face, f->nnames, TT_NAME_ID_FULL_NAME);
    f->family_name = get_name(f->face, f->nnames, TT_NAME_ID_FONT_FAMILY);
    /* XXX: the following is wrong */
    f->notice = get_name(f->face, f->nnames, TT_NAME_ID_COPYRIGHT);

    if (what & WHAT_FONT) {
	if (read_dir(f, fname) != 0) {
	    close_font(f);
	    return NULL;
	}
    }
    if (what & WHAT_AFM) {
	if ((err=TT_New_Instance(f->face, &f->fi)) != TT_Err_Ok) {
	    fprintf(stderr, "%s: %s: can't create font instance: %s\n",
		    prg, fname, TT_ErrToString18(err));
	    close_font(f);
	    return NULL;
	}

	if (TT_New_Glyph(f->face, &f->fg) != TT_Err_Ok) {
	    fprintf(stderr, "%s: %s: can't create glyph container: %s\n",
		    prg, fname, TT_ErrToString18(err));
	    close_font(f);
	    return NULL;
	}
    }

    return f;
}



static char *
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



static int
read_dir(font *f, char *fname)
{
    static char *table_name[] = {
	"cvt ", "fpgm", "glyf", "head", "hhea", "hmtx", "loca", "maxp", "prep"
    };

    int ntables, i, j;
    TT_Long len;
    struct table *dir;
    unsigned char bh[12], *b;

    len = 12;
    TT_Get_Font_Data(f->face, 0, 0, bh, &len);

    ntables = (bh[4]<<8)+bh[5];

    dir = (struct table *)xmalloc(sizeof(struct table)*ntables);
    b = (unsigned char *)xmalloc(ntables*16);

    len = ntables*16;
    TT_Get_Font_Data(f->face, 0, 12, b, &len);

    for (i=0; i<16*ntables; i+=16) {
	memcpy(dir[i/16].tag, b+i, 4);
	dir[i/16].tag[4] = '\0';
	dir[i/16].checksum = (((((b[i+4]<<8)+b[i+5])<<8)+b[i+6])<<8)+b[i+7];
	dir[i/16].offset = (((((b[i+8]<<8)+b[i+9])<<8)+b[i+10])<<8)+b[i+11];
	dir[i/16].length = (((((b[i+12]<<8)+b[i+13])<<8)+b[i+14])<<8)+b[i+15];
    }
    free(b);

    for (i=j=0; i<ntables && j<9; i++) {
	if (strcmp(dir[i].tag, table_name[j]) == 0) {
	    f->dir[j] = dir[i];

	    if (j != TABLE_GLYF && f->dir[j].length > MAX_STRLEN) {
		fprintf(stderr, "%s: %s: table `%s' too long\n",
		    prg, fname, f->dir[j].tag);
		free(dir);
		return -1;
	    }
	    
	    j++;
	}
    }
    free(dir);

    /* XXX: check for missing table */

    return 0;
}

