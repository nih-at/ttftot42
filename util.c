/*
  util.c -- miscellaneous functions
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
#include <stdlib.h>

#include "config.h"

#ifdef HAVE_FREETYPE_FREETYPE_H
#include <freetype/ftxpost.h>
#include <freetype/ftxerr18.h>
#else
#include <ftxpost.h>
#include <ftxerr18.h>
#endif

#include "t42.h"

static void write_strdef(FILE *fout, char *n, char *s);
static void write_string(FILE *fout, char *s);

static int write_tabledir_entry(FILE *fout, struct table *t);
static int write_table(FILE *f, unsigned char *b, unsigned long length,
		       int *strlen, int *offset);
static int write_glyf(FILE *fout, font *f, unsigned char *b,
		      unsigned char *loca, unsigned long length,
		      int *slen, int *offset);
static unsigned char *read_table(font *f, unsigned long offset,
				 unsigned long length);




int
write_font_info(FILE *fout, font *f)
{
    fputs("/FontInfo 9 dict dup begin\n", fout);
    write_strdef(fout, "version", f->version);
    write_strdef(fout, "Notice", f->notice);
    write_strdef(fout, "FullName", f->full_name);
    write_strdef(fout, "FamilyName", f->family_name);
    write_strdef(fout, "Weight", f->weight);
    fprintf(fout, "/ItalicAngle %s def\n", f->italic_angle);
    fprintf(fout, "/isFixedPitch %s def\n",
	    f->is_fixed_pitch ? "true" : "false");
    fprintf(fout, "/UnderlinePosition %d def\n", f->underline_position);
    fprintf(fout, "/UnderlineThickness %d def\n", f->underline_thickness);
    fputs("end readonly def\n", fout);

    return ferror(fout);
}



static void
write_strdef(FILE *fout, char *n, char *s)
{
    if (s == NULL)
	return;
    
    fprintf(fout, "/%s (", n);
    write_string(fout, s);
    fprintf(fout, ") readonly def\n");
}



static void
write_string(FILE *fout, char *s)
{
    int i, l, p;

    l = strlen(s);

    p = 0;
    for (i=0; i<l; i++) {
	if (s[i] == '(')
	    p++;
	else if (s[i] == ')')
	    if (--p < 0)
		break;
    }

    if (p == 0)
	fputs(s, fout);
    else
	for (i=0; i<l; i++) {
	    if (s[i] == '(' || s[i] == ')')
		putc('\\', fout);
	    putc(s[i], fout);
	}

    return;
}



#define FIXED_SUM  0xB4DED201    /* checksum of fixpart and table tags */

int
write_sfnts(font *f, FILE *fout)
{
    static char *fixpart = "/sfnts[<\n00010000""0009""0080""0003""0010\n";

    int i, slen, soff;
    unsigned long offset, checksum;
    struct table wdir[NTABLES];  /* dir ready for writing */
    unsigned char *loca, *b;

    fputs(fixpart, fout);

    offset = 0;
    for (i=0; i<NTABLES; i++) {
	wdir[i] = f->dir[i];
	wdir[i].offset = offset+HEADER_LEN;
	offset += (wdir[i].length+3) & ~3;
    }
    
    /* compute font checksum for head table */

    checksum = FIXED_SUM;
    for (i=0; i<NTABLES; i++) {
	write_tabledir_entry(fout, wdir+i);
	checksum += wdir[i].checksum*2 + wdir[i].offset + wdir[i].length;
    }
    checksum &= 0xffffffff;
    checksum = 0xb1b0afba - checksum;
    checksum &= 0xffffffff;

    soff = 0;
    slen = HEADER_LEN;
    loca = NULL;
    for (i=0; i<NTABLES; i++) {
	if (i != TABLE_LOCA || loca == NULL)
	    b = read_table(f, f->dir[i].offset, wdir[i].length);
	else
	    b = loca;

	if (i == TABLE_HEAD) {
	    b[8] = (checksum>>24)&0xff;
	    b[9] = (checksum>>16)&0xff;
	    b[10] = (checksum>>8)&0xff;
	    b[11] = checksum&0xff;
	}
	if (i == TABLE_GLYF && wdir[i].length > MAX_STRLEN) {
	    loca = read_table(f, f->dir[TABLE_LOCA].offset,
			      wdir[TABLE_LOCA].length);
	    write_glyf(fout, f, b, loca, wdir[i].length,
			&slen, &soff);
	}
	else
	    write_table(fout, b, wdir[i].length, &slen, &soff);
	free(b);
    }

    if (soff % LINE_LEN != 0)
	fputc('\n', fout);
    fputs("00>]def\n", fout);
    /* printf("DEBUG: slen=%d\n", slen); */

    return 0;
}



static int
write_tabledir_entry(FILE *fout, struct table *t)
{
    fprintf(fout, "%2.2X%2.2X%2.2X%2.2X%8.8lX%8.8lX%8.8lX\n",
	    t->tag[0], t->tag[1], t->tag[2], t->tag[3],
	    t->checksum & 0xffffffff,
	    t->offset & 0xffffffff,
	    t->length & 0xffffffff);

    return 0;
}



static int
write_table(FILE *fout, unsigned char *b, unsigned long length,
	    int *slen, int *offset)
{
    int i;
    if (*slen+length > MAX_STRLEN) {
	if (*offset % LINE_LEN != 0)
	    fputc('\n', fout);
	/* printf("DEBUG: *slen=%d\n", *slen); */
	fputs("00><\n", fout);
	*slen = *offset = 0;
    }
    
    for (i=0; i<length; i++) {
	fprintf(fout, "%02X", b[i]);
	if (++(*offset) % LINE_LEN == 0)
	    fputc('\n', fout);
    }

    if (length % 4 != 0) {
	for (i=0; i<4-(length%4); i++) {
	    fputs("00", fout);
	    if (++(*offset) % LINE_LEN == 0)
		fputc('\n', fout);
	}
    }
    
    *slen += (length+3) & ~3;

    return 0;
}



static int
write_glyf(FILE *fout, font *f, unsigned char *b, unsigned char *loca,
	   unsigned long length, int *slen, int *offset)
{
    int i, len;
    unsigned long off, start, off_old, j;

    len = *slen;
    start = 0;
    off_old = 0;
    for (i=0; i<=f->nglyph; i++) {
	if (f->long_loc)
	    off = (((((loca[i*4]<<8)+loca[i*4+1])<<8)
		    +loca[i*4+2])<<8)+loca[i*4+3];
	else
	    off = ((loca[i*2]<<8)+loca[i*2+1])*2;
	if (len+(off-start) > MAX_STRLEN) {
	    for (j=start; j<off_old; j++) {
		fprintf(fout, "%02X", b[j]);
		if (++(*offset) % LINE_LEN == 0)
		    fputc('\n', fout);
	    }
	    if (*offset % LINE_LEN != 0)
		fputc('\n', fout);
	    fputs("00><\n", fout);
	    /* printf("DEBUG: len=%d\n", (int)(len+off_old-start)); */
	    len = *offset = 0;

	    start = off_old;
	}
	off_old = off;
    }

    /* doesn't work if the whole glyf table fits in existing string */
    for (j=start; j<length; j++) {
	fprintf(fout, "%02X", b[j]);
	if (++(*offset) % LINE_LEN == 0)
	    fputc('\n', fout);
    }
    if (length % 4 != 0) {
	for (i=0; i<4-(length%4); i++) {
	    fputs("00", fout);
	    if (++(*offset) % LINE_LEN == 0)
		fputc('\n', fout);
	}
    }
    *slen = ((length+3) & ~3)-start;

    return 0;
}



static unsigned char *
read_table(font *f, unsigned long offset, unsigned long length)
{
    TT_Long len;
    unsigned char *b;

    b = (unsigned char *)xmalloc(length);
    len = length;
    
    TT_Get_Font_Data(f->face, 0, offset, b, &len);
    
    return b;
}



int
get_encoding(font *f, struct encoding *enc)
{
    int i, valid, err;
    TT_CharMap cmap;
    unsigned short pl, en, idx;
    char **vector;

    /* XXX: include font file name in error messages */

    valid = 0;
    for (i=0; i<f->ncmaps; i++) {
	TT_Get_CharMap_ID(f->face, i, &pl, &en);
	/* XXX: don't hardcode pid/eid */
	if ((pl == TT_PLATFORM_MACINTOSH && en == TT_MAC_ID_ROMAN)) {
	    if ((err=TT_Get_CharMap(f->face, i, &cmap)) != TT_Err_Ok) {
		fprintf(stderr, "%s: can't get cmap: %s\n",
			prg, TT_ErrToString18(err));
		return -1;
	    }
	    valid = 1;
	    break;
	}
    }

    if (!valid) {
	fprintf(stderr, "%s: no compatible cmap found\n", prg);
	return -1;
    }

    vector = xmalloc(sizeof(char *)*256);

    for (i=0; i<256; i++) {
	if ((idx=TT_Char_Index(cmap, i)) != 0) {
	    if (TT_Get_PS_Name(f->face, idx, vector+i) != TT_Err_Ok) {
		/* XXX: handle error? */
		vector[i] = NULL;
	    }
	}
	else
	    vector[i] = NULL;
    }

    enc->vector = vector;
    return 0;
}



void
clear_encoding(struct encoding *enc)
{
    if ((enc->flags & ENC_FROMFONT )== 0)
	return;

    if (enc->vector) {
#if 0
	/* XXX: are strings returned by TT_Get_PS_Name malloc()ed? */
	for (i=0; i<256; i++)
	    free(enc->vector[i]);
#endif
	free(enc->vector);
	enc->vector = NULL;
    }
    if (enc->reverse) {
	free(enc->reverse);
	enc->reverse = NULL;
    }
}
