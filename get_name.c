/*
  get_name -- get ISO Latin 1 name string from name table
  Copyright (C) 1998 Dieter Baron

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
#include <string.h>
#include <freetype.h>

/*
  apple unicode 0
  
  mac 1
    roman 0
      english 0
      french 1
      german 2
      italian 3
      dutch 4
      swedish 5
      spanish 6
      danish 7
      portugese 8 
      norwegian 9

  iso 2
  
  win 3
    unicode 1
*/

char *
get_name(TT_Face f, int nnames, int name)
{
    int i;
    TT_UShort pid, eid, lid, nid, len;
    char *p, *s, *t;

    /* XXX: if (nnames == 0) set nnames */

    for (i=0; i<nnames; i++) {
	TT_Get_Name_ID(f, i, &pid, &eid, &lid, &nid);

	if (nid == name) {
	    if (((pid == TT_PLATFORM_MACINTOSH && eid == TT_MAC_ID_ROMAN)
		 || (pid == TT_PLATFORM_MICROSOFT
		     && eid == TT_MS_ID_UNICODE_CS))) {
		TT_Get_Name_String(f, i, &p, &len);
		if (pid == TT_PLATFORM_MACINTOSH) {
		    if ((s=(char *)malloc(len+1)) == NULL)
			return NULL;
		    strncpy(s, p, len);
		    s[len] = '\0';
		    return s;
		}
		else {
		    if ((s=(char *)malloc((len/2)+1)) == NULL)
			return NULL;
		    for (t=s; len; p+=2,len-=2) {
			if (*p == 0)
			    *(t++) = p[1];
		    }
		    *t = '\0';
		    return s;
		}
	    }
	}
    }

    return NULL;
}
