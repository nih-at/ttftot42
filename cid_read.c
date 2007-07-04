/*
  cid_read -- read CID files
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
#include <stdlib.h>
#include <string.h>

#include "t42.h"
#include "cid.h"

#define get_short(p)		((*(p))+=2, (*((*(p))-2)<<8)+(*((*(p))-1)))
static int get_long(unsigned char **p);
static char *get_string(unsigned char **p);

static char *read_portion(FILE *f);



struct cid *
cid_read(char *fname)
{
    struct cid *cid;
    struct cid_cmap *cmap;
    unsigned char b[6], *p, *q;
    int i, j;
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
    cid->supl_ncid = (int *)xmalloc(sizeof(int)*cid->nsupl);
    for (i=0; i<cid->nsupl; i++)
	cid->supl_ncid[i] = get_long(&p);


    cid->ncmap = get_short(&p);
    cid->cmap = (struct cid_cmap *)xmalloc(sizeof(struct cid_cmap)*cid->ncmap);

    free(q);

    for (i=0; i<cid->ncmap; i++) {
	if ((q=p=read_portion(f)) == NULL) {
	    /* error: read error */
	    cid->ncmap = i;
	    cid_free(cid);
	    fclose(f);
	    return NULL;
	}

	cmap = cid->cmap+i;

	cmap->pid = get_short(&p);
	cmap->eid = get_short(&p);

	cmap->nvert = get_long(&p);
	cmap->vert = (struct cid_feature *)xmalloc(sizeof(struct cid_feature)
						   * cmap->nvert);
	for (i=0; i<cmap->nvert; i++) {
	    cmap->vert[i].script = get_long(&p);
	    cmap->vert[i].language = get_long(&p);
	    cmap->vert[i].feature = get_long(&p);
	}
	cmap->nfeature = get_long(&p);
	cmap->feature = (struct cid_feature *)xmalloc(sizeof(struct
							     cid_feature)
						      * cmap->nfeature);
	for (i=0; i<cmap->nfeature; i++) {
	    cmap->feature[i].script = get_long(&p);
	    cmap->feature[i].language = get_long(&p);
	    cmap->feature[i].feature = get_long(&p);
	}

	cmap->code = (struct cid_code *)xmalloc(sizeof(struct cid_code));
	cmap->code->nchar = get_long(&p);
	cmap->code->ndata = cmap->code->size = get_long(&p);

	cmap->code->data = (unsigned short *)xmalloc(sizeof(unsigned short)
						    * cmap->code->ndata);
	for (j=0; j<cmap->code->ndata; j++)
	    cmap->code->data[j] = get_short(&p);

	free(q);
    }

    fclose(f);

    return cid;
}



static int
get_long(unsigned char **p)
{
    int i;

    i = (**p << 24) + (*((*p)+1) << 16) + (*((*p)+2) << 8) + (*((*p)+3));
    *p += 4;

    return i;
}



static char *
get_string(unsigned char **p)
{
    int l;
    char *s;

    l = get_short(p);
    s = (char *)xmalloc(l);
    memcpy(s, *p, l);
    *p += l;

    return s;
}



static char *
read_portion(FILE *f)
{
    int len;
    unsigned char *data, b[4];

    if (fread(b, 1, 4, f) != 4) {
	/* error: read error */
	return NULL;
    }

    data = b;
    len = get_long(&data);

    data = (char *)xmalloc(len);

    if (fread(data, 1, len, f) != len) {
	/* error: read error */
	free(data);
	return NULL;
    }

    return data;
}
