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
#include <time.h>
#include <ftxpost.h>

#include "config.h"
#include "t42.h"

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


    /* XXX: sfnts: head hhea loca maxp cvt_ prep glyf hmtx fpgm */

    fputs("FontName currentdict end definefont pop\n", fout);

    return 0;
}
