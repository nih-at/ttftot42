/*
  write_t42 -- write Type 42 font file
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
#include <time.h>

#include "config.h"

#ifdef HAVE_FREETYPE_FREETYPE_H
#include <freetype/ftxpost.h>
#else
#include <ftxpost.h>
#endif

#include "t42.h"



int
write_t42(font *f, FILE *fout, struct encoding *encoding)
{
    static char *fixpart = "\
/FontType 42 def\n\
/FontMatrix [1 0 0 1 0 0] def\n";

    int i, had_notdef;
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

    write_font_info(fout, f);

    /* encoding */

    if (encoding->flags & ENC_BUILTIN)
	fprintf(fout, "/Encoding %s def\n", encoding->full_name);
    else {
	/* XXX: put only glyph actually contained in font in encoding array */
	fputs("/Encoding 256 array\n", fout);
	fputs("0 1 255 { 1 index exch /.notdef put } for\n", fout);
	for (i=0; i<256; i++) {
	    if (encoding->vector[i])
		fprintf(fout, "dup %d /%s put\n", i, encoding->vector[i]);
	}
	fputs("readonly def\n", fout);
    }


    /* CharStrings */

    had_notdef = 0;
    fprintf(fout, "/CharStrings %d dict dup begin\n", f->nglyph);
    for (i=0; i<f->nglyph; i++) {
	/* XXX: ensure syntactic correctness */
	TT_Get_PS_Name(f->face, i, &name);
	if (strcmp(name, ".notdef") == 0) {
	    if (!had_notdef)
		had_notdef = 1;
	    else
		continue;
	}
	fprintf(fout, "/%s %d def\n", name, i);
    }
    fputs("end readonly def\n", fout);

    write_sfnts(f, fout);

    fputs("FontName currentdict end definefont pop\n", fout);

    return 0;
}



