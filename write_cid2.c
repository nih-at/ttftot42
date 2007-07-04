/*
  write_t42 -- write Type 42 CID font file (type 11)
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

    fprintf(fout, "%%%%BeginResource: CIDFont (%s)\n", f->font_name);
    fprintf(fout, "%%%%Title: (%s %s %s %d)\n",
	    f->font_name, cid->registry, cid->ordering, cid->supplement);

    fputs("15 dict begin\n", fout);
    fprintf(fout, "/CIDFontName /%s def\n", f->font_name);

    fputs(fixpart, fout);

    fprintf(fout, "/FontBBox [%d %d %d %d] def\n",
	    f->font_bbox.llx, f->font_bbox.lly,
	    f->font_bbox.urx, f->font_bbox.ury);
    

    /* FontInfo */

    write_font_info(fout, f);

    
    /* CIDSystemInfo */

    fputs("/CIDSystemInfo 3 dict dup begin\n", fout);
    fprintf(fout, "/Registry (%s) def\n", cid->registry);
    fprintf(fout, "/Ordering (%s) def\n", cid->ordering);
    fprintf(fout, "/Supplement %d def\n", cid->supplement);
    fputs("end def\n", fout);


    /* XXX: UIDBase */
    /* XXX: WMode */
    /* XXX: XUID */

    
    /* glyph id to cid mapping */

    write_cidmap(f, cid, fout);
    

    write_sfnts(f, fout);


    /* for Type 42 compatibility: */

    /*  CharStrings */
    fputs("/CharStrings 1 dict dup begin\n", fout);
    fputs("/.notdef 0 def\n",fout);
    fputs("end readonly def\n", fout);

    /*  Encoding */
    fputs("/Encoding StandardEncoding def\n", fout);


    /* trailer */

    fputs("CIDFontName currentdict end /CIDFont defineresource pop\n\
%%EndResource\n\
%%EOF\n", fout);

    return 0;
}



int
write_cidmap(font *f, struct cid *cid2, FILE *fout)
{
    int i;
    int ncid, slen;
    unsigned short *map;

    if ((ncid=cid_mkmap(f, cid2, &map)) < 0)
	return -1;

    fputs("/GDBytes 2 def\n", fout);

    fputs("/CIDMap [<\n", fout);

    slen = 0;
    for (i=0; i<ncid; i++) {
	if (slen >= MAX_STRLEN) {
	    fputs("\n> <\n", fout);
	    slen = 0;
	}

	fprintf(fout, "%04hx", map[i]);
	slen += 2;
	if (slen % 36 == 0)
	    putc('\n', fout);
    }

    fputs(">] def\n", fout);

    fprintf(fout, "/CIDCount %d def\n", ncid);

    free(map);

    return 0;
}   
