/*
  cid_write -- write CID files
  Copyright (C) 1999 Dieter Baron

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
    int len;
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

    write_short(f, len);
    write_string(f, cid->registry);
    write_string(f, cid->ordering);
    write_short(f, cid->nsupl);
    for (i=0; i<cid->nsupl; i++)
	write_long(f, cid->supl_ncid[i]);
    write_short(f, cid->ncmap);


    /* maps */

    for (i=0; i<cid->ncmap; i++) {
	cmap = cid->cmap[i];
	
	len = 28 + 12*cmap->nfeature + 2*cmap->code->len;

	write_short(f, len);
	write_short(f, cmap->pid);
	write_short(f, cmap->eid);
	write_long(f, cid->vert.script);
	write_long(f, cid->vert.language);
	write_long(f, cid->vert.feature);
	write_long(f, cmap->nfeature);
	for (j=0; j<cmap->nfeature; j++) {
	    write_long(f, cid->feature[i].script);
	    write_long(f, cid->feature[i].language);
	    write_long(f, cid->feature[i].feature);
	}
	write_long(f, cmap->code->nchar);
	write_long(f, cmap->code->len);
	for (j=0; j<cmap->code->len; j++)
	    write_short(f, cmap->code->data[j]);
    }

    /* XXX: write error */

    fclose(f);

    return 0;
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
	putc('\0', c);
}
