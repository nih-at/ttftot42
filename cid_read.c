/*
  cid_read -- read CID files
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
#include <stdlib.h>

#include "cid.h"

#define get_short(p)		((*(p))+=2, *((*(p))-2)<<8+*((*(p))-1))
static int get_int(unsigned char **p);
static int get_string(unsigned char **p);



struct cid *
read_cid(char *fname)
{
    struct cid *cid;
    struct cid_cmap *cmap;
    unsigned char b[6], *p, *q;
    int i, temp;
    FILE *f;

    if ((f=fopen(fname, "rb")) == NULL) {
	/* error: can't open file */
	return NULL;
    }

    if (fread(b, 1, 6, f) != 6) {
	/* error: read error or file size < pre-header */
	fclose(f);
	return NULL;
    }

    if (strncmp(b, "CID0", 4) != 0) {
	/* error: not a cid file */
	fclose(f);
	return NULL;
    }

    if (b[4] != 1) {
	/* error: major version incompatibility */
	return NULL;
    }

    if (b[5] > 0) {
	/* warning: minor version incompatibility */
    }

    cid = cid_new();
    cid->v_major = 1;
    cid->v_minor = 0;

    if ((q=p=read_portion(f)) == NULL) {
	/* error: read error */
	cid_free(cid);
	fclose(f);
	return NULL;
    }

    cid->registry = get_string(&p);
    cid->ordering = get_string(&p);
    cid->supplement = get_short(&p);
    cid->nsupl = cid->supplement+1;
    cid->supl_ncid = (int *)malloc(sizeof(int)*cid->nsupl);
    for (i=0; i<cid->nsupl; i++)
	cid->supl_ncid[i] = get_int(&p);


    cid->ncmap = get_short(&p);
    cid->cmap = (struct cid_cmap *)malloc(sizeof(struct cid_map)*cid->ncmap);

    free(q);

    for (i=0; i<cid->ncmap; i++) {
	if ((q=p=read_portion(f)) == NULL) {
	    /* error: read error */
	    cid->ncmaps = i;
	    cid_free(cid);
	    fclose(f);
	    return NULL;
	}

	cmap = cid->cmap[i];

	cmap->pid = get_short(&p);
	cmap->eid = get_short(&p);
	cmap->vert.script = get_long(&p);
	cmap->vert.language = get_long(&p);
	cmpa->vert.feature = get_long(&p);

	cmap->nfeature = get_short(&p);
	cmap->feature = (struct cid_feature *)malloc(sizeof(struct
							    cid_feature *)
						     * nfeature);
	for (i=0; i<cmap->nfeature; i++) {
	    cmap->feature[i].script = get_long(&p);
	    cmap->feature[i].language = get_long(&p);
	    cmap->feature[i].feature = get_long(&p);
	}

	cmap->code = (struct cid_code *)malloc(sizeof(struct cid_code));
	cmap->code->nchar = get_short(&p);
	cmap->code->len = cmap->code->size = get_short(&p);

	cmap->code->data = (unsigned short *)malloc(sizeof(unsigned short)
						    * cmap->code->len);
	for (j=0; j<cmap->code->len; j++)
	    cmap->code->data[j] = get_short(&p);

	free(q);
    }

    fclose(f);

    return cid;
}



static int
get_int(unsigned char **p)
{
    int i;

    i = **p << 24 + *((*p)+1) << 16 + *((*p)+2) << 8 + *((*p)+3);;
    *p += 4;

    return i;
}



static int
get_string(unsigned char **p)
{
    int l;
    char *s;

    l = get_short(p);
    s = (char *)malloc(l);
    memcpy(s, *p, l);
    *p += l;

    return s;
}

