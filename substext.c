/*
  substext.c -- make substitution of exension of filename
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



#include <string.h>

#include "substext.h"



char *
substext(char *fname, char *ext, char *newext)
{
    static char b[8192];
    int l, el;

    l = strlen(fname);
    el = strlen(ext);

    if (strcasecmp(fname+l-el, ext) == 0) {
	strncpy(b, fname, l-el);
	strcpy(b+l-el, newext);
    }
    else {
	strcpy(b, fname);
	strcpy(b+l, newext);
    }

    return b;
}
