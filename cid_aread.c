/*
  cid_aread -- read ACID files
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
#include <string.h>
#include <stdarg.h>

#include "t42.h"
#include "cid.h"
#include "pideid.h"

#define GET_TOKEN()	(p=strtok(NULL, " \t\n"),\
			 (p==NULL ? error("command %s missing argument", b),0\
                                  : 0))

enum state {
    ST_BEGIN,	/* before StartCID */
    ST_CID,	/* inside Start(End)CID */
    ST_CMAPS,	/* inside Start(End)Charmaps */
    ST_CMAP,	/* inside Start(End)Charmap */
    ST_ENC,	/* inside Start(End)Encoding */
    ST_END	/* after EndCID */
};

void cid_encode_init(struct cid_code *code);
int cid_encode(struct cid_code *code, char *line);
void cid_encode_flush(struct cid_code *code);


static int lineno;
static char *filename;

static void error(char *fmt, ...);
static void warning(char *fmt, ...);

static int make_tag(unsigned char *tag);

static void grow(struct cid_code *code, int idx);
static void store_cmd(struct cid_code *code, int cmd);
static void store_arg(struct cid_code *code, int arg);



struct cid *
cid_aread(char *fname)
{
    FILE *f;
    struct cid *cid;
    struct cid_cmap *cmap;
    struct cid_feature *feature;
    enum state state;
    char b[8192], *cmd, *p, *q;
    int i, j;
    int ccmap, ncmap, garbage, enc, ma, mi, next_cid;

    if ((f=fopen(fname, "r")) == NULL) {
	/* error: can't open file */
	return NULL;
    }

    filename = fname;

    state = ST_BEGIN;
    garbage = enc = 0;
    ccmap = ncmap = 0;

    while (fgets(b, 8192, f)) {
	lineno++;
	cmd = strtok(b, " \t\n");
	if (cmd == NULL)
	    continue;
	if (state != ST_BEGIN && strcmp(cmd, "Comment") == 0)
	    continue;

	switch (state) {
	case ST_BEGIN:
	    if (strcmp(cmd, "StartCID") == 0) {
		GET_TOKEN();
		ma = strtol(p, &q, 10);
		if (*q != '.') {
		    error("malformed version number");
		    return NULL;
		}
		mi = strtol(q+1, &p, 10);
		if (*p != '\0') {
		    error("malformed version number");
		    return NULL;
		}

		if (ma != 1) {
		    error("major version incompatibility (%d > 1)", ma);
		    return NULL;
		}
		if (mi != 0)
		    warning("minor version incompatibility (%d > 0)", mi);

		cid = cid_new();
		cid->v_major = 1;
		cid->v_minor = 0;

		state = ST_CID;
	    }
	    else {
		error("not a ACID file");
		return NULL;
	    }
	    break;
		
	case ST_CID:
	    if (strcmp(cmd, "Registry") == 0) {
		GET_TOKEN();
		cid->registry = strdup(p);
	    }
	    else if (strcmp(cmd, "Ordering") == 0) {
		GET_TOKEN();
		cid->ordering = strdup(p);
	    }
	    else if (strcmp(cmd, "Supplements") == 0) {
		GET_TOKEN();
		cid->supplement = atoi(p);
		cid->nsupl = cid->supplement+1;
		cid->supl_ncid = (int *)xmalloc(sizeof(int)*cid->nsupl);

		for (i=0; i<cid->nsupl; i++) {
		    GET_TOKEN();
		    cid->supl_ncid[i] = atoi(p);
		}
	    }
	    else if (strcmp(cmd, "StartCharmaps") == 0) {
		if (ncmap) {
		    error("multiple Charmaps sections");
		    cid_free(cid);
		    return NULL;
		}
		GET_TOKEN();
		ncmap = atoi(p);
		if (ncmap == 0) {
		    error("at least one Charmap required");
		    cid_free(cid);
		    return NULL;
		}
		cid->cmap = (struct cid_cmap *)xmalloc(sizeof(struct cid_cmap)
						       * ncmap);

		state = ST_CMAPS;
	    }
	    else if (strcmp(cmd, "StartEncoding") == 0) {
		if (ncmap == 0) {
		    error("Charmaps section required before Encoding section");
		    cid_free(cid);
		    return NULL;
		}
		if (enc) {
		    error("multiple Encoding sections");
		    cid_free(cid);
		    return NULL;
		}
		enc = 1;

		for (i=0; i<cid->ncmap; i++)
		    cid_encode_init(cid->cmap[i].code);
		next_cid = 0;
		
		state = ST_ENC;
	    }
	    else if (strcmp(cmd, "EndCID") == 0) {
		if (ncmap == 0) {
		    error("Charmaps section missing");
		    cid_free(cid);
		    return NULL;
		}
		if (enc == 0) {
		    error("Encoding section missing");
		    cid_free(cid);
		    return NULL;
		}

		state = ST_END;
	    }
	    else
		warning("unknown keyword `%s' ignored\n", cmd);

	    break;

	case ST_CMAPS:
	    if (strcmp(cmd, "StartCharmap") == 0) {
		if (ccmap >= ncmap) {
		    cid->cmap
			= (struct cid_cmap *)xrealloc(cid->cmap,
						      (sizeof(struct cid_cmap)
						       * ccmap+1));
		}
		cid->ncmap++;
		cmap = cid->cmap+ccmap;
		GET_TOKEN();
		cmap->pid = otf_str2pid(p);
		if (cmap->pid == -1)
		    warning("unknown Platform ID `%s', Charmap ignored", p);
		GET_TOKEN();
		cmap->eid = otf_str2eid(cmap->pid, p);
		if (cmap->eid == -1)
		    warning("unknown Encoding ID `%s', Charmap ignored", p);
		cmap->nvert = cmap->nfeature = 0;
		cmap->vert = cmap->feature = NULL;
		cmap->code = cid_code_new();

		state = ST_CMAP;
	    }
	    else if (strcmp(cmd, "EndCharmaps") == 0) {
		if (cid->ncmap < ncmap)
		    warning("%d Charmaps declared but only %d defined\n",
			    ncmap, ccmap);
		if (cid->ncmap > ncmap)
		    warning("only %d Charmaps declared but %d defined\n",
			    ncmap, ccmap);
		
		state = ST_CID;
	    }
	    else
		warning("unknown keyword `%s' ignored (inside Charmaps)", cmd);

	    break;

	case ST_CMAP:
	    if ((strcmp(cmd, "Vertical") == 0
		 || strcmp(cmd, "Feature") == 0)) {
		if (strcmp(cmd, "Vertical") == 0) {
		    cmap->vert
			= ((struct cid_feature *)
			   xrealloc(cmap->vert, (sizeof(struct cid_feature)
						    * (cmap->nvert+1))));
		    feature = cmap->vert+cmap->nvert;
		    cmap->nvert++;
		}
		else {
		    cmap->feature
			= ((struct cid_feature *)
			   xrealloc(cmap->feature, (sizeof(struct cid_feature)
						    * (cmap->nfeature+1))));
		    feature = cmap->feature+cmap->nfeature;
		    cmap->nfeature++;
		}
		GET_TOKEN();
		feature->script = make_tag(p);
		GET_TOKEN();
		feature->language = make_tag(p);
		GET_TOKEN();
		feature->feature = make_tag(p);
	    }
	    else if (strcmp(cmd, "EndCharmap") == 0) {
		ccmap++;

		state = ST_CMAPS;
	    }
	    else
		warning("unknown keyword `%s' ignored (inside Charmap)", cmd);
	    break;

	case ST_ENC:
	    if (strcmp(cmd, "CID") == 0) {
		GET_TOKEN();
		/* allow for missing cid values (treat as `*') */
		i = atoi(p);
		while (next_cid < i) {
		    for (j=0; j<cid->ncmap; j++)
			cid_encode(cid->cmap[i].code, "*");
		    next_cid++;
		}
		next_cid = i+1;
		
		for (i=0; i<cid->ncmap; i++) {
		    GET_TOKEN();
		    cid_encode(cid->cmap[i].code, p);
		}
	    }
	    else if (strcmp(cmd, "EndEncoding") == 0) {
		for (i=0; i<cid->ncmap; i++)
		    cid_encode_flush(cid->cmap[i].code);

		state = ST_CID;
	    }
	    else
		warning("unknown keyword `%s' ignored (inside Encoding)", cmd);

	    break;

	case ST_END:
	    if (!garbage) {
		warning("garbage after EndCID ignored");
		garbage = 1;
	    }
	}
    }

    fclose(f);

    /* XXX: post processing (check pid/eid) , state == ST_END */

    return cid;
}



void
cid_encode_init(struct cid_code *code)
{
    code->size = code->ndata = 0;
    code->nchar = 0;
    code->data = NULL;

    code->cmd = C_none;
    code->idx = 0;
    code->len = 0;
}



int
cid_encode(struct cid_code *code, char *line)
{
    int vvec, vtype, len, bit, v, vert;
    char *q;

    if (line[0] == '*') {
	if (code->cmd != C_N || code->len >= 0x1fff) {
	    cid_encode_flush(code);
	    code->cmd = C_N;
	}
	code->len++;
    }
    else if (strchr(line, ',')) {
	cid_encode_flush(code);
	vvec = vtype = len = 0;
	bit = 1;
	while (line) {
	    v = strtol(line, &q, 16);
	    vert = (*q == 'v');
	    store_arg(code, v);
	    if (vert) {
		vvec |= bit;
		vtype |= 2;
	    }
	    else
		vtype |= 1;
	    
	    bit <<= 1;
	    if ((line = strchr(line, ',')))
		line++;
	}

	switch (vtype) {
	case 1:
	    store_cmd(code, C_A);
	    break;
	case 2:
	    store_cmd(code, C_AV);
	    break;
	case 3:
	    if (code->len > 16) {
		warning("mixed alternatives of length > 16 can't be encoded, "
			"rest dropped");
		code->len = 16;
	    }
	    grow(code, code->idx+code->len+2);
	    memmove(code->data+code->idx+2, code->data+code->idx+1,
		    code->len*sizeof(short));
	    code->data[code->idx+1] = vvec;
	    store_cmd(code, C_AM);
	    break;
	}
    }
    else {
	v = strtol(line, &q, 16);
	vert = (*q == 'v');

	if ((code->len < 0x1fff
	     && ((vert && code->cmd == C_OV) || (!vert && code->cmd == C_O))
	     && code->data[code->idx+1]+code->len == v)) {
	    code->len++;
	}
	else if ((code->len >= 0x1fff
		  || (vert && code->cmd != C_LV)
		  || (!vert && code->cmd != C_L))) {
	    cid_encode_flush(code);
	    code->cmd = vert ? C_LV : C_L;
	    store_arg(code, v);
	}
	else if ((code->len > 1
		  && code->data[code->idx+code->len] == v-1
		  && code->data[code->idx+code->len-1] == v-2)) {
	    code->len -= 2;
	    if (code->len)
		cid_encode_flush(code);
	    code->cmd = vert ? C_OV : C_O;
	    store_arg(code, v-2);
	    code->len = 3;
	}
	else
	    store_arg(code, v);
    }

    return 0;
}



void
cid_encode_flush(struct cid_code *code)
{
    if (code->cmd != C_none)
	store_cmd(code, code->cmd);
}



static void
store_arg(struct cid_code *code, int arg)
{
    int idx;

    code->len++;
    idx = code->idx + code->len;

    grow(code, idx);

    code->data[idx] = arg;
}



static void
store_cmd(struct cid_code *code, int cmd)
{
    grow(code, code->idx);

    code->data[code->idx] = cmd | code->len;
    
    switch(cmd) {
    case C_N:
	code->idx += 1;
	break;

    case C_O:
    case C_OV:
	code->idx += 2;
	break;

    case C_L:
    case C_LV:
    case C_A:
    case C_AV:
	code->idx += code->len + 1;
	break;

    case C_AM:
	code->idx += code->len + 2;
	break;
    }

    code->len =  0;
    code->ndata = code->idx;
    code->cmd = C_none;
}



static void
grow(struct cid_code *code, int idx)
{
    if (idx >= code->size) {
	code->size += 1024;
	code->data = (unsigned short *)xrealloc(code->data,
						sizeof(unsigned short *)
						* code->size);
    }
}



static int
make_tag(unsigned char *tag)
{
    int l;

    l = strlen(tag);

    return (((l>0 ? tag[0] : ' ') << 24)
	    + ((l>1 ? tag[1] : ' ') << 16)
	    + ((l>2 ? tag[2] : ' ') << 8)
	    + (l>3 ? tag[3] : ' '));
}



static void
error(char *fmt, ...)
{
    va_list va;

    fprintf(stderr, "%s: %s:%d: ", prg, filename, lineno);

    va_start(va, fmt);
    vfprintf(stderr, fmt, va);
    va_end(va);

    putc('\n', stderr);
}



static void
warning(char *fmt, ...)
{
    va_list va;

    fprintf(stderr, "%s: %s:%d: warning: ", prg, filename, lineno);

    va_start(va, fmt);
    vfprintf(stderr, fmt, va);
    va_end(va);

    putc('\n', stderr);
}

