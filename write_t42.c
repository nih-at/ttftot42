/*
  write_t42 -- write Type 42 font file
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



