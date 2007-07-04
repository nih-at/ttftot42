/*
  cid_write -- write CID files
  Copyright (C) 1999 Dieter Baron

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

#include "cid.h"

#define write_short(f, s)	(putc((s)>>8, (f)), putc((s)&0xff, (f)))
#define write_long(f, s)	(putc((s)>>24, (f)), \
				 putc(((s)>>16)&0xff, (f)), \
				 putc(((s)>>8)&0xff, (f)), \
				 putc((s)&0xff,(f)))

static void write_string(FILE *f, char *s);



int
cid_write(char *fname, struct cid *cid)
{
    FILE *f;
    int len, i, j;
    struct cid_cmap *cmap;

    if (cid->v_major != 1) {
	/* error: major version incompatibility */
	return -1;
    }

    if (cid->v_minor > 0) {
	/* warning: minor version incompatibility */
    }

    if ((f=fopen(fname, "wb")) == NULL) {
	/* error: can't create file */
	return -1;
    }

    /* pre header */

    fprintf(f, "CID0%c%c", 1, 0);

    
    /* header */

    len = strlen(cid->registry)+3;
    if (len%2 == 1)
	len++;
    len += strlen(cid->ordering)+3;
    if (len%2 == 1)
	len++;
    len += 2 + cid->nsupl*4 + 2;

    write_long(f, len);
    write_string(f, cid->registry);
    write_string(f, cid->ordering);
    write_short(f, cid->nsupl-1);
    for (i=0; i<cid->nsupl; i++)
	write_long(f, cid->supl_ncid[i]);
    write_short(f, cid->ncmap);


    /* maps */

    for (i=0; i<cid->ncmap; i++) {
	cmap = cid->cmap+i;
	
	len = 20 + 12*cmap->nvert + 12*cmap->nfeature + 2*cmap->code->ndata;

	write_long(f, len);
	write_short(f, cmap->pid);
	write_short(f, cmap->eid);
	write_long(f, cmap->nvert);
	for (j=0; j<cmap->nvert; j++) {
	    write_long(f, cmap->vert[i].script);
	    write_long(f, cmap->vert[i].language);
	    write_long(f, cmap->vert[i].feature);
	}
	write_long(f, cmap->nfeature);
	for (j=0; j<cmap->nfeature; j++) {
	    write_long(f, cmap->feature[i].script);
	    write_long(f, cmap->feature[i].language);
	    write_long(f, cmap->feature[i].feature);
	}
	write_long(f, cmap->code->nchar);
	write_long(f, cmap->code->ndata);
	for (j=0; j<cmap->code->ndata; j++)
	    write_short(f, cmap->code->data[j]);
    }

    /* XXX: write error */

    return fclose(f);
}



static void
write_string(FILE *f, char *s)
{
    int len, pad;

    len = strlen(s)+1;
    pad = len % 2;

    write_short(f, len+pad);
    fprintf(f, "%s%c", s, '\0');
    if (pad)
	putc('\0', f);
}
