/*
  write_t42 -- write Type 42 CID font file (type 11)
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
#include <ftxpost.h>

#include "config.h"
#include "t42.h"



int
write_cid2(font *f, FILE *fout, struct cid *cid)
{
    static char *fixpart = "\
/CIDFontType 2 def\n\
/FontType 42 def\n\
/FontMatrix [1 0 0 1 0 0] def\n";

    time_t fuck_ctime;

    fputs("%!PS-Adobe-3.0 Resource-CIDFont\n", fout);

    time(&fuck_ctime);
    fputs("%%Creator: " PACKAGE " " VERSION "\n", fout);
    fprintf(fout, "%%%%CreationDate: %s", ctime(&fuck_ctime));

    fputs("15 dict begin\n", fout);
    fprintf(fout, "/CIDFontName /%s def\n", f->font_name);

    fputs(fixpart, fout);

    fprintf(fout, "/FontBBox [%d %d %d %d] def\n",
	    f->font_bbox.llx, f->font_bbox.lly,
	    f->font_bbox.urx, f->font_bbox.ury);
    

    /* FontInfo */

    write_font_info(fout, f);

    
    /* CIDSystemInfo */

    /* XXX: handle other collections */
    fputs("/CIDSystemInfo 3 dict dup begin\n", fout);
    fprintf(fout, "/Registry (%s) def\n", "Adobe");
    fprintf(fout, "/Ordering (%s) def\n", "Japan1");
    fprintf(fout, "/Supplement %d def\n", 2);
    fputs("end def\n", fout);


    /* XXX: UIDBase */
    /* XXX: WMode */
    /* XXX: XUID */

    
    /* glyph id to cid mapping */

    write_cidmap(f, fout);
    

    write_sfnts(f, fout);


    /* for Type 42 compatibility: */

    /*  CharStrings */
    fputs("/CharStrings 1 dict dup begin\n", fout);
    fputs("/.notdef 0 def\n",fout);
    fputs("end readonly def\n", fout);

    /*  Encoding */
    fputs("/Encoding StandardEncoding def\n", fout);


    /* trailer */

    fputs("CIDFontName currentdict end /CIDFont defineresource pop\n", fout);

    return 0;
}





