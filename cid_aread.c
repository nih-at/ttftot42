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
#include <string.h>

#include "cid.h"

enum state {
    ST_BEGIN,	/* before StartCID */
    ST_CID,	/* inside Start(End)CID */
    ST_CMAPS,	/* inside Start(End)Charmaps */
    ST_CMAP,	/* inside Start(End)Charmap */
    ST_ENC,	/* inside Start(End)Encoding */
    ST_END	/* after EndCID */
};



struct cid *
cid_aread(char *fname)
{
    FILE *f;
    struct cid *cid;
    struct cid_cmap *cmap;
    struct feature *feature;
    FILE *f;
    unsigned char b[8192], *p, *q;
    int i, j, lineno;
    int ccmap, ncmap;

    if ((f=fopen(fname, "r")) == NULL) {
	/* error: can't open file */
	return NULL;
    }

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
		    error("major version incompatibility (%d > 1)", major);
		    return NULL;
		}
		if (mi != 0)
		    warning("minor version incompatibility (%d > 0)", minor);

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
		cid->supplement = atio(p);
		cid->nsupl = cid->supplement+1;
		cid->ncid_sup = (int *)xmalloc(sizeof(int)*cid->nsupl);

		for (i=0; i<=cid->nsupl; i++) {
		    GET_TOKEN();
		    cid->ncid_sup[i] = atio(p);
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
		cid->cmap = (struct cmap *)xmalloc(sizeof(struct cmap)*ncmap);

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
		GET_TOKEN();
		enc = atoi(p);
		if (enc == 0) {
		    error("at least one CID required");
		    cid_free(cid);
		    return NULL;
		}
		/* XXX: missing */
		
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
			= (struct cid_cmap *)xrealloc(sizeof(struct cid_cmap)
						      * ccmap+1);
		}
		cid->ncmap++;
		cmap = cid->cmap[ccmap];
		GET_TOKEN();
		cmap->pid = get_pid(p);
		GET_TOKEN();
		cmap->eid = get_eid(cid->cmap[cmap].pid, p);
		cmap->vert.script = 0;
		cmap->vert.language = 0;
		cmap->vert.feature = 0;
		cmap->nfeature = 0;
		cmap->feature = NULL;
		cmap->code = NULL;

		state = ST_CMAP;
	    }
	    else if (strcmp(cmd, "EndCharmaps") == 0) {
		if (cid->ncmap < ncmap)
		    warning("%d Charmaps declared but only %d defined\n"
			    ncmap, ccmap);
		if (cid->ncmap > ncmap)
		    warning("only %d Charmaps declared but %d defined\n"
			    ncmap, ccmap);
		
		state = ST_CID;
	    }
	    else
		warning("unknown keyword `%s' ignored (inside Charmaps)", cmd);

	    break;

	case ST_CMAP:
	    if ((strcmp(cmp, "Vertical") == 0
		 || strcmp(cmp, "Feature") == 0)) {
		if (strcmp(cmp, "Vertical") == 0)
		    feature = &cmap->vert;
		else {
		    cmap->feature
			= (struct feature *)xrealloc(cmap->feature,
						     (sizeof(struct feature)
						      * cmap->nfeature+1));
		    feature = cmap->feature+cmap->nfeature;
		    cmap->nfeature++;
		}
		GET_TOK();
		feature->script = make_tag(p);
		GET_TOK();
		feature->language = make_tag(p);
		GET_TOK();
		feature->feature = make_tag(p);
	    }
	    else if (strcmp(cmp, "EndCharmap") == 0) {
		ccmap++;

		state = ST_CMAPS;
	    }
	    else
		warning("unknown keyword `%s' ignored (inside Charmap)", cmd);
	    break;

	case ST_ENC:
	    if (strcmp(cmd, "CID") == 0) {
		/* XXX: missing */
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

    /* XXX: post processing (check pid/eid) */
    
    return cid;
}
