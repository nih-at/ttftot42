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

struct cmap_state {
    int valid;
    TT_CharMap cmap;
    /* features */
};



int
cid_mkmap(struct font *f, struct cid *cid, unsigned short **mapp)
{
    struct cmap_state *state;
    unsigned short *map;
    int valid, v, flags;
    int i, j, ncid;
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
	if (state[i].valid)
	    cid_decode_init(cid->cmap[i].code);
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
			map[i] = v;
			ncid = i;
		    }
		}
	    } while (flags & CID_ALT);
	}
    }

    *mapp = map;
    return ncid+1;
}
