#ifndef _HAD_SUBSTEXT_H
#define _HAD_SUBSTEXT_H

/*
  substext.h -- make substitution of exension of filename
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



#include "config.h"

#if (defined(HAVE_LIBGEN_H) && defined(HAVE_BASENAME))
#include <libgen.h>
#endif

#ifndef HAVE_BASENAME
char *basename(char *name);
#endif



char *substext(char *fname, char *ext, char *newext);

#endif /* substext.h */
