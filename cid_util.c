/*
  cid_util.c -- utility functions for cid structures
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
