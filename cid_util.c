/*
  cid_util.c -- utility functions for cid structures
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



#include <stdlib.h>

#include "t42.h"



struct cid *
cid_new(void)
{
    struct cid *cid;

    cid = (struct cid *)xmalloc(sizeof(struct cid));

    cid->registry = cid->ordering = NULL;
    cid->supplement = cid->v_major = cid->v_minor = 0;
    cid->nsupl = cid->ncmap = 0;
    cid->supl_ncid = NULL;
    cid->cmap = NULL;

    return cid;
}



void
cid_free(struct cid *cid)
{
    int i;

    if (cid == NULL)
	return;

    free(cid->registry);
    free(cid->ordering);
    free(cid->supl_ncid);
    
    for (i=0; i<cid->ncmap; i++) {
	if (cid->cmap[i].nvert)
	    free(cid->cmap[i].vert);
	if (cid->cmap[i].nfeature)
	    free(cid->cmap[i].feature);
	free(cid->cmap[i].code->data);
	free(cid->cmap[i].code);
    }
    if (cid->ncmap)
	free(cid->cmap);

    free(cid);
}



struct cid_code *
cid_code_new(void)
{
    struct cid_code *code;

    code = (struct cid_code *)xmalloc(sizeof(struct cid_code));

    code->size = code->ndata = 0;
    code->data = NULL;

    return code;
}
