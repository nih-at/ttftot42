/*
  write_afm -- write AFM file
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
#include <limits.h>
#include <time.h>

#include "config.h"

#ifdef HAVE_FREETYPE_FREETYPE_H
#include <freetype/ftxpost.h>
#include <freetype/ftxkern.h>
#else
#include <ftxpost.h>
#include <ftxkern.h>
#endif

#include "t42.h"

struct glyph {
    char *name;
    int index;
    int code;
};

struct kern {
    char *n1, *n2;
    int k;
};

typedef int (*compfunc)(const void *, const void *);
typedef int (*searchfunc) (const void *, const void *);

static int make_revenc(struct encoding *encoding);
static int make_glyph(font *f, struct encoding *enc, struct glyph **glyphp);
static int make_kern(font *f, struct kern **kernp);



int
write_afm(font *f, FILE *fout, struct encoding *encoding)
{
    static char *fixpart = "\
StartFontMetrics 4.1\n\
Comment Creator: " PACKAGE " " VERSION "\n";

    TT_Glyph_Metrics metrics;
    struct glyph *glyph;
    struct kern *kern;
    int nkern, nglyph;
    time_t fuck_ctime;
    int i;

    fputs(fixpart, fout);
    time(&fuck_ctime);
    fprintf(fout, "Comment Creation Date: %s", ctime(&fuck_ctime));

    /* XXX: /UniqueID, XUID */

    if (f->vm_min != 0 && f->vm_max != 0)
	fprintf(fout, "Comment VMusage: %d %d\n",
		f->vm_min, f->vm_max);

    fprintf(fout, "FontName %s\n", f->font_name);
    fprintf(fout, "FullName %s\n", f->full_name);
    fprintf(fout, "FamilyName %s\n", f->family_name);
    fprintf(fout, "Weight %s\n", f->weight);
    fprintf(fout, "ItalicAngle %s\n", f->italic_angle);
    fprintf(fout, "IsFixedPitch %s\n", f->is_fixed_pitch ? "true" : "false");
    fprintf(fout, "FontBBox %d %d %d %d\n",
	    f->font_bbox.llx, f->font_bbox.lly,
	    f->font_bbox.urx, f->font_bbox.ury);
    fprintf(fout, "UnderlinePosition %d\n", f->underline_position);
    fprintf(fout, "UnderlineThickness %d\n", f->underline_thickness);
    fprintf(fout, "Version %s\n", f->version);
    fprintf(fout, "Notice %s\n", f->notice);
    if (strcmp(encoding->full_name, "StandardEncoding") == 0)
	fputs("EncodingScheme AdobeStandardEncoding\n", fout);
    else
	fprintf(fout, "EncodingScheme %s\n", encoding->full_name);
#if 0
    /* XXX: are not provided by FreeType */
    fprintf(fout, "CapHeight %d\n", f->capheight);
    fprintf(fout, "XHeight %d\n", f->xheight);
#endif
    fprintf(fout, "Ascender %d\n", f->ascender);
    fprintf(fout, "Descender %d\n", f->descender);
    /* XXX: StdHW */
    /* XXX: StdVW */


    /* char metrics */

    make_revenc(encoding);
    nglyph = make_glyph(f, encoding, &glyph);
    fprintf(fout, "StartCharMetrics %d\n", nglyph);
    for (i=0; i<nglyph; i++) {
        TT_Load_Glyph(f->fi, f->fg, glyph[i].index, 0);
        TT_Get_Glyph_Metrics(f->fg, &metrics);
	
	fprintf(fout, "C %d ; WX %d ; N %s ; B %d %d %d %d ;\n",
		glyph[i].code != INT_MAX ? glyph[i].code : -1,
		SCALE(metrics.advance), glyph[i].name,
		SCALE(metrics.bbox.xMin), SCALE(metrics.bbox.yMin),
		SCALE(metrics.bbox.xMax), SCALE(metrics.bbox.yMax));
    }
    free(glyph);

    fputs("EndCharMetrics\n", fout);


    /* kern data */

    nkern = make_kern(f, &kern);

    if (nkern) {
	fputs("StartKernData\n", fout);
	/* StartTrackKern */
	/* EndTrackKern */
	fprintf(fout, "StartKernPairs %d\n", nkern);

	for (i=0; i<nkern; i++) {
	    if (i != 0 && strcmp(kern[i-1].n1, kern[i].n1) != 0)
		fputc('\n', fout);
	    fprintf(fout, "KPX %s %s %d\n",
		    kern[i].n1, kern[i].n2, kern[i].k);
	}
	    
	fputs("EndKernPairs\n", fout);
	fputs("EndKernData\n", fout);

	free(kern);
    }
    
    /* StartComposites */
    /* EndComposites */

    fputs("EndFontMetrics\n", fout);

    return 0;
}



static int
code_cmp(struct rev_enc *c1, struct rev_enc *c2)
{
    return strcmp(c1->name, c2->name);
}

static int
make_revenc(struct encoding *encoding)
{
    int i, j;

    /* XXX: encodings with multiply encoded glyphs are not handled properly */

    if (encoding->reverse != NULL)
	return 0;

    encoding->reverse = (struct rev_enc *)xmalloc(256*sizeof(struct rev_enc));

    for (i=j=0; i<256; i++) {
	if (encoding->vector[i]) {
	    encoding->reverse[j].name = encoding->vector[i];
	    encoding->reverse[j++].code = i;
	}
    }
    encoding->nreverse = j;

    qsort(encoding->reverse, j, sizeof(struct rev_enc), (compfunc)code_cmp);

    return 0;
}



static int
glyph_cmp(struct glyph *c1, struct glyph *c2)
{
    if (c1->code == c2->code)
	return strcmp(c1->name, c2->name);
    else
	return c1->code - c2->code;
}

static int
code_find(char *k, struct rev_enc *c)
{
    return strcmp(k, c->name);
}

static int
make_glyph(font *f, struct encoding *enc, struct glyph **g)
{
    int i, j;
    struct glyph *glyph;
    struct rev_enc *c;

    if ((glyph=(struct glyph *)xmalloc(sizeof(struct glyph)*f->nglyph)) == NULL)
	return 0;
    
    for (j=i=0; i<f->nglyph; i++) {
	TT_Get_PS_Name(f->face, i, &glyph[j].name);
	if (strcmp(glyph[j].name, ".notdef") == 0)
	    continue;
	glyph[j].index = i;
	c = bsearch(glyph[j].name, enc->reverse, enc->nreverse,
		    sizeof(struct rev_enc), (searchfunc)code_find);
	if (c)
	    glyph[j].code = c->code;
	else
	    glyph[j].code = INT_MAX;
	j++;
    }

    qsort(glyph, j, sizeof(struct glyph), (compfunc)glyph_cmp);

    *g = glyph;
    return j;
}



static int
kern_cmp(struct kern *k1, struct kern *k2)
{
    int c;

    c = strcmp(k1->n1, k2->n1);
    if (c == 0)
	return strcmp(k1->n2, k2->n2);
    return c;
}

static int
make_kern(font *f, struct kern **k)
{
    int nkern, i, j;
    char *n1, *n2;
    struct kern *kern;

    TT_Kerning kd;
    TT_Kern_0 k0;

    if (TT_Get_Kerning_Directory(f->face, &kd) != TT_Err_Ok)
	return 0;

    for (i=0; i<kd.nTables; i++) {
	if (kd.tables[i].format == 0) {
	    if (TT_Load_Kerning_Table(f->face, i) != TT_Err_Ok)
		continue;

	    k0 = kd.tables[i].t.kern0;

	    if ((kern=(struct kern *)xmalloc(sizeof(struct kern)*k0.nPairs))
		== NULL)
		return 0;
	    
	    for (j=i=0; i<k0.nPairs; i++) {
		if (k0.pairs[i].value == 0)
		    continue;

		TT_Get_PS_Name(f->face, k0.pairs[i].left, &n1);
		TT_Get_PS_Name(f->face, k0.pairs[i].right, &n2);

		if (strcmp(n1, ".notdef") == 0 || strcmp(n2, ".notdef") == 0)
		    continue;

		kern[j].n1 = n1;
		kern[j].n2 = n2;
		kern[j].k = k0.pairs[i].value;
		j++;
	    }
	    nkern = j;
	    qsort(kern, nkern, sizeof(struct kern), (compfunc)kern_cmp);

	    *k = kern;
	    return nkern;
	}
    }

    return 0;
}
