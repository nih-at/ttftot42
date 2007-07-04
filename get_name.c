/*
  get_name -- get ISO Latin 1 name string from name table
  Copyright (C) 1998, 1999 Dieter Baron

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
#include <string.h>

#include "t42.h"

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
	    if (((pid == TT_PLATFORM_MACINTOSH
		  && eid == TT_MAC_ID_ROMAN
		  && lid == TT_MAC_LANGID_ENGLISH)
		 || (pid == TT_PLATFORM_MICROSOFT
		     && (eid == TT_MS_ID_UNICODE_CS || 
			 eid == TT_MS_ID_SYMBOL_CS)))) {
		TT_Get_Name_String(f, i, &p, &len);
		if (pid == TT_PLATFORM_MACINTOSH) {
		    if ((s=(char *)xmalloc(len+1)) == NULL)
			return NULL;
		    strncpy(s, p, len);
		    s[len] = '\0';
		    return s;
		}
		else {
		    /* XXX: does TT_MS_ID_SYMBOL_CS have 2 byte chars? */
		    if ((s=(char *)xmalloc((len/2)+1)) == NULL)
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
