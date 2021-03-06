/*
  open_font -- open font file and extract needed information
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



#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

#include "config.h"

#ifdef HAVE_FREETYPE_FREETYPE_H
#include <freetype/ftxpost.h>
#include <freetype/ftxerr18.h>
#else
#include <ftxpost.h>
#include <ftxerr18.h>
#endif

#include "t42.h"

static char *weight_name[] = {
    "Thin", "Extra Light", "Light", "Regular", "Medium",
    "Semibold", "Bold", "Extra Bold", "Black"
};
int nweight = sizeof(weight_name)/sizeof(weight_name[0]);

static char *fixed2str(TT_Fixed f, int prec);
static int read_dir(font *f, char *fname);



font *
open_font(char *fname, int what, int type)
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

    if ((f=(font *)xmalloc(sizeof(font))) == NULL)
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

#ifdef HAVE_GSUB
    f->gsub = NULL;
    if (type & TYPE_CID) {
	TTO_GSUBHeader foo;

	TT_Load_GSUB_Table(face, &foo);
	TT_Extension_Get( face.z,
			  ( ((unsigned long)('G') << 24) |
			    ((unsigned long)('S') << 16) |
			    ((unsigned long)('U') << 8 ) |
			    (unsigned long)('B') ), (void**)&f->gsub );
    }
#endif
    
    /* fill out info */

    f->face = face;
    f->nglyph = prop.num_Glyphs;
    f->nnames = prop.num_Names;
    f->ncmaps = prop.num_CharMaps;
    f->long_loc = hdr->Index_To_Loc_Format;
    f->units_per_em = hdr->Units_Per_EM;
    
    f->font_bbox.llx = SCALE(hdr->xMin);
    f->font_bbox.lly = SCALE(hdr->yMin);
    f->font_bbox.urx = SCALE(hdr->xMax);
    f->font_bbox.ury = SCALE(hdr->yMax);

    f->vm_min = ps->minMemType42;
    f->vm_max = ps->maxMemType42;
    if ((ps->italicAngle>>16) <= 180 && (ps->italicAngle>>16) >= -180)
	f->italic_angle = fixed2str(ps->italicAngle, 3);
    else
	f->italic_angle = strdup("0");
    /* XXX: scale values from post table? */
    f->underline_position = ps->underlinePosition-ps->underlineThickness/2;
    f->underline_thickness = ps->underlineThickness;
    f->is_fixed_pitch = ps->isFixedPitch;

    weight = (os2->usWeightClass-50)/100;
    if (weight >= nweight)
	weight = nweight-1;
    f->weight = strdup(weight_name[weight]);
    f->ascender = SCALE(os2->sTypoAscender);
    f->descender = SCALE(os2->sTypoDescender);
#if 0
    /* XXX: are not provided by FreeType */
    f->capheight = SCALE(os2->sCapHeight);
    f->xheight = SCALE(os2->sxHeight);
#endif

    f->tt_version = strdup("001.000"); /* FreeType only supports version 1.0 */
    f->version = (char *)xmalloc(8);
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
    
    f->full_name = get_name(f->face, f->nnames, TT_NAME_ID_FULL_NAME);
    f->font_name = get_name(f->face, f->nnames, TT_NAME_ID_PS_NAME);
    if (f->font_name == NULL) {
	/* Fall back to full name if no PS name specified. */
	if (f->full_name == NULL) {
	    fprintf (stderr, "%s: %s: missing FontName and FullName\n",
		     prg, fname);
	    close_font(f);
	    return NULL;
	}
	fprintf (stderr, "%s: %s: warning: missing FontName\n", prg, fname);
	f->font_name = strdup(f->full_name);
    }
    /* Remove illegal characters from the font name. 
       Actually, the spec only requires spaces to be removed. */
    for (j=i=0; i<strlen(f->font_name); i++) {
	if (!strchr(" \t\n\r()<>[]/%", f->font_name[i]))
	    f->font_name[j++] = f->font_name[i];
    }
    f->font_name[j] = '\0';
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

    for (i=j=0; i<ntables && j<NTABLES; i++) {
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

