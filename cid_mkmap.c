/*
  cid_mkmap.c -- make array of glyph ids from cid
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

#include "t42.h"

struct cmap_feature {
    TT_UShort sidx, lidx, fidx;
};

struct cmap_state {
    int valid;
    TT_CharMap cmap;
    int nvert, nfeature;
    struct cmap_feature *vert, feature;
};

#ifdef HAVE_GSUB
static int get_feature(font *f, struct cid_feature *feature,
		       struct cmap_feature *feat)
#endif



int
cid_mkmap(struct font *f, struct cid *cid, unsigned short **mapp)
{
    struct cmap_state *state;
    unsigned short *map;
    int valid, v, flags;
    int i, j, ncid;
#ifdef HAVE_GSUB
    int k;
#endif
    unsigned short pl, en;

    state = (struct cmap_state *)xmalloc(sizeof(struct cmap_state)*cid->ncmap);

    valid = 0;
    for (i=0; i<cid->ncmap; i++) {
	state[i].valid = 0;
	for (j=0; j<f->ncmaps; j++) {
	    TT_Get_CharMap_ID(f->face, j, &pl, &en);
	    if ((pl == cid->cmap[i].pid && en == cid->cmap[i].eid)) {
		if (TT_Get_CharMap(f->face, i, &state[i].cmap) == TT_Err_Ok)
		    valid = state[i].valid = 1;
		break;
	    }
	}
	if (state[i].valid) {
	    cid_decode_init(cid->cmap[i].code);
#ifdef HAVE_GSUB
	    if (cid->cmap[i].nvert)
		state[i].vert
		    = (struct cmap_feature *)xmalloc(sizeof(struct
							    cmap_feature)
						     * cid->cmap[i].nvert);
	    else
		state[i].vert = NULL;
	    if (cid->cmap[i].nfeature)
		state[i].feature
		    = (struct cmap_feature *)xmalloc(sizeof(struct
							    cmap_feature)
						     * cid->cmap[i].nfeature);
	    else
		state[i].feature = NULL;
	    state[i].nvert = state[i].nfeature = 0;
	    for (k=0; k<cid->cmap[i].nvert; k++) {
		if (get_feature(f->face,
				cid->cmap[i].vert+k,
				state[i].vert+state[i].nvert) == 0)
		    state[i].nvert++;
	    }
	    for (k=0; k<cid->cmap[i].nfeature; k++) {
		if (get_feature(f->face,
				cid->cmap[i].feature+k,
				state[i].feature+state[i].nfeature) == 0)
		    state[i].nfeature++;
	    }
#endif /* HAVE_GSUB */
	}
    }

    if (valid == 0) {
	/* XXX: no suitable charmap found */
	return -1;
    }

    map = (unsigned short *)xmalloc(sizeof(unsigned short)
				    * cid->supl_ncid[cid->supplement]);

    ncid = 0;
    for (i=0; i<cid->supl_ncid[cid->supplement]; i++) {
	map[i] = 0;
	for (j=0; j<cid->ncmap; j++) {
	    do {
		flags = cid_decode(cid->cmap[j].code, &v);
		if ((flags & CID_NENC) == 0 && state[j].valid && map[i] == 0) {
		    if ((v=TT_Char_Index(state[j].cmap, v)) != 0) {
			/* XXX: apply vert & feature */
#ifdef HAVE_GSUB
			if (flags & CID_VERT) {
			    v = apply_features(f, state[j].nvert,
					       state[j].vert, v);
			}
			v = apply_features(f, state[j].nfeature,
					   state[j].feature, v);
			map[i] = v;
			ncid = i;
#else /* HAVE_GSUB */
			if (!(flags & CID_VERT)) {
			    map[i] = v;
			    ncid = i;
			}
#endif
		    }
		}
	    } while (flags & CID_ALT);
	}
    }

    *mapp = map;
    
#ifdef HAVE_GSUB
    for (i=0; i<cid->ncmap; i++) {
	if (state[i].valid) {
	    free(state[i].vert);
	    free(state[i].feature);
	}
    }
#endif HAVE_GSUB

    return ncid+1;
}



#ifdef HAVE_GSUB

static int
get_feature(font *f, struct cid_feature *feature, struct cmap_feature *feat)
{
    if (TT_GSUB_Select_Script(f->gsub, feature->script, &feat->sidx))
	return -1;
    if (TT_GSUB_Select_Language(f->gsub, feature->language,
				feat->sidx, &feat->lidx))
	return -1;
    if (TT_GSUB_Select_Feature(f->gsub, feature->feature,
			       feat->sidx, feat->lidx, &feat->fidx))
	return -1;
    return 0;
}



static int
apply_features(font *f, int n, struct cmap_feature *feat, int v)
{
    /* XXX: FreeType 1.2 doesn't implement TTO_GSUB_Apply */
    return v;
}
#endif /* HAVE_GSUB */

