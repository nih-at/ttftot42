/*
  write_t42 -- write Type 42 font file
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
#include <stdlib.h>
#include <time.h>
#include <ftxpost.h>

#include "config.h"
#include "t42.h"

struct table {
    char tag[5];
    unsigned long checksum;
    unsigned long offset;
    unsigned long length;
};

static int read_dir(font *f, struct table **dirp);
static unsigned char *read_table(font *f, unsigned long offset,
				 unsigned long length);
static int write_sfnts(font *f, FILE *fout);
static int write_tabledir_entry(FILE *fout, struct table *t);
static int write_table(FILE *f, unsigned char *b, unsigned long length,
		       unsigned long *strlen, unsigned long *offset);



int
write_t42(font *f, FILE *fout)
{
    static char *fixpart = "\
/FontType 42 def\n\
/FontMatrix [1 0 0 1 0 0] def\n";

    int i;
    char *name;
    time_t fuck_ctime;

    fprintf(fout, "%%!PS-TrueTypeFont-%s-%s\n",
	    f->tt_version, f->version);

    time(&fuck_ctime);
    fputs("%%Creator: " PACKAGE " " VERSION "\n", fout);
    fprintf(fout, "%%%%CreationDate: %s", ctime(&fuck_ctime));

    if (f->vm_min != 0 && f->vm_max != 0)
	fprintf(fout, "%%%%VMusage: %d %d\n",
		f->vm_min, f->vm_max);

    fputs("11 dict begin\n", fout);
    fprintf(fout, "/FontName /%s def\n", f->font_name);
    
    fputs(fixpart, fout);

    fprintf(fout, "/FontBBox [%d %d %d %d] def\n",
	    f->font_bbox.llx, f->font_bbox.lly,
	    f->font_bbox.urx, f->font_bbox.ury);
    
    /* XXX: /PaintType {0|2} */
    /* XXX: /StrokeWidth (for PaintType 2) */
    fputs("/PaintType 0 def\n", fout);
    /* XXX: /UniqueID, XUID */


    /* fontinfo */

    fputs("/FontInfo 9 dict dup begin\n", fout);
    fprintf(fout, "/version (%s) readonly def\n", f->version);
    fprintf(fout, "/Notice (%s) readonly def\n", f->notice);
    fprintf(fout, "/FullName (%s) readonly def\n", f->full_name);
    fprintf(fout, "/FamilyName (%s) readonly def\n", f->family_name);
    fprintf(fout, "/Weight (%s) readonly def\n", f->weight);
    fprintf(fout, "/ItalicAngle %s def\n", f->italic_angle);
    fprintf(fout, "/isFixedPitch %s def\n",
	    f->is_fixed_pitch ? "true" : "false");
    fprintf(fout, "/UnderlinePosition %d def\n", f->underline_position);
    fprintf(fout, "/UnderlineThickness %d def\n", f->underline_thickness);
    fputs("end readonly def\n", fout);
    

    /* encoding */

    /* XXX: handle other encodings */
    fprintf(fout, "/Encoding StandardEncoding def\n");


    /* CharStrings */

    fprintf(fout, "/CharStrings %d dict dup begin\n", f->nglyph);
    for (i=0; i<f->nglyph; i++) {
	TT_Get_PS_Name(f->face, i, &name);
	fprintf(fout, "/%s %d def\n", name, i);
    }
    fputs("end readonly def\n", fout);

    write_sfnts(f, fout);

    fputs("FontName currentdict end definefont pop\n", fout);

    return 0;
}



#define MAX_STRLEN  65534    /* max. PostScript string length - 1 */
#define LINE_LEN    36       /* length of one line (in bytes) */
#define HEADER_LEN  12+9*16  /* length of header and table directory */
#define TABLE_GLYF  2        /* index of glyf table in table_names */

static int
write_sfnts(font *f, FILE *fout)
{
    static char *table_name[] = {
	"cvt ", "fpgm", "glyf", "head", "hhea", "hmtx", "loca", "maxp", "prep"
    };
    static char *fixpart = "/sfnts[<\n00010000""0009""0080""0003""0010\n";

    int ntables, i, j;
    unsigned long offset, length;
    struct table *fdir;    /* full dir */
    struct table sdir[9];  /* shortened dir */
    struct table wdir[9];  /* dir ready for writing */
    unsigned char *head, *b;

    fputs(fixpart, fout);

    ntables = read_dir(f, &fdir);

    offset = 0;
    for (i=j=0; i<ntables && j<9; i++) {
	if (strcmp(fdir[i].tag, table_name[j]) == 0) {
	    sdir[j] = fdir[i];
	    wdir[j] = fdir[i];
	    wdir[j].offset = offset+HEADER_LEN;
	    offset += (sdir[j].length+3) & ~3;

	    j++;
	}
    }
    /* XXX: check for missing table */

    /* XXX: adjust head checksum & font checksum in head table */

    for (i=0; i<9; i++)
	write_tabledir_entry(fout, wdir+i);

    /* XXX: special treatment of long glyf table */
    offset = 0;
    length = HEADER_LEN;
    for (i=0; i<9; i++) {
	b = read_table(f, sdir[i].offset, sdir[i].length);
	write_table(fout, b, sdir[i].length, &length, &offset);
    }

    if (offset % LINE_LEN != 0)
	fputc('\n', fout);
    fputs("00>]def\n", fout);

    return 0;
}



static int
write_tabledir_entry(FILE *fout, struct table *t)
{
    fprintf(fout, "%02.2X%02.2X%02.2X%02.2X%08.8lX%08.8lX%08.8lX\n",
	    t->tag[0], t->tag[1], t->tag[2], t->tag[3],
	    t->checksum, t->offset, t->length);

    return 0;
}



static int
write_table(FILE *fout, unsigned char *b, unsigned long length,
	    unsigned long *strlen, unsigned long *offset)
{
    int i;
    if (*strlen+length > MAX_STRLEN) {
	if (*offset % LINE_LEN != 0)
	    fputc('\n', fout);
	fputs("00><\n", fout);
	*strlen = *offset = 0;
    }
    /* XXX: check for overly long tables */
    
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
    
    *strlen += (length+3) & ~3;

    return 0;
}




static int
read_dir(font *f, struct table **dirp)
{
    int ntables, i;
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

    *dirp = dir;
    return ntables;
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
