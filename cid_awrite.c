/*
  cid_awrite -- write ACID files
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
#include "pideid.h"

static void print_feature(FILE *f, struct cid_feature *feature);
static char *tag2str(unsigned long tag);



int
cid_awrite(char *fname, struct cid *cid)
{
    FILE *f;
    int i, j, flags, v;

    if (cid->v_major != 1) {
	/* error: major version incompatibility */
	return -1;
    }

    if (cid->v_minor > 0) {
	/* warning: minor version incompatibility */
    }

    if ((f=fopen(fname, "w")) == NULL) {
	/* error: can't create file */
	return -1;
    }

    fprintf(f, "StartCID %d.%d\n", 1, 0);
    fprintf(f, "Registry %s\n", cid->registry);
    fprintf(f, "Ordering %s\n", cid->ordering);
    fprintf(f, "Supplements %d", cid->nsupl-1);
    for (i=0; i<cid->nsupl; i++)
	fprintf(f, " %d", cid->supl_ncid[i]);
    putc('\n', f);
    
    fprintf(f, "StartCharmaps %d\n", cid->ncmap);
    for (i=0; i<cid->ncmap; i++) {
	fprintf(f, "StartCharmap %s %s\n",
		otf_pid2str(cid->cmap[i].pid),
		otf_eid2str(cid->cmap[i].pid, cid->cmap[i].eid));
	for (j=0; j<cid->cmap[i].nvert; j++) {
	    fprintf(f, "Vertical ");
	    print_feature(f, cid->cmap[i].vert+j);
	}
	for (j=0; j<cid->cmap[i].nfeature; j++) {
	    fprintf(f, "Feature ");
	    print_feature(f, cid->cmap[i].feature+j);
	}
	fprintf(f, "EndCharmap\n");
    }
    fprintf(f, "EndCharmaps\n");

    fprintf(f, "StartEncoding %d\n", cid->supl_ncid[cid->nsupl-1]);
    for (j=0; j<cid->ncmap; j++)
	cid_decode_init(cid->cmap[j].code);
    for (i=0; i<cid->supl_ncid[cid->nsupl-1]; i++) {
	fprintf(f, "CID %d", i);
	for (j=0; j<cid->ncmap; j++) {
	    putc(' ', f);
	    do {
		flags = cid_decode(cid->cmap[j].code, &v);
		if (flags & CID_NENC)
		    fprintf(f, "*");
		else
		    fprintf(f, "%04x%s%s",v,
			    flags & CID_VERT ? "v" : "",
			    flags & CID_ALT ? "," : "");
	    } while (flags & CID_ALT);
	}
	putc('\n', f);
    }

    fprintf(f, "EndEncoding\n");

    fprintf(f, "EndCID\n");

    return fclose(f);
}



static void
print_feature(FILE *f, struct cid_feature *feature)
{
    fprintf(f, "%s", tag2str(feature->script));
    fprintf(f, " %s", tag2str(feature->language));
    fprintf(f, " %s\n", tag2str(feature->feature));
}



static char *
tag2str(unsigned long tag)
{
    static unsigned char b[5];
    int i;

    b[0] = tag>>24;
    b[1] = (tag>>16) & 0xff;
    b[2] = (tag>>8) & 0xff;
    b[3] = (tag) & 0xff;

    for (i=3; b[i] == ' '; --i)
	;
    b[i+1] = '\0';

    return b;
}
