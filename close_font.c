/*
  close_font -- free font structure
  Copyright (C) 1998, 1999 Dieter Baron

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
#include <limits.h>

#include "config.h"

#ifdef HAVE_FREETYPE_FREETYPE_H
#include <freetype/ftxpost.h>
#include <freetype/ftxerr18.h>
#else
#include <ftxpost.h>
#include <ftxerr18.h>
#endif

#include "t42.h"

void
close_font(font *f)
{
    free(f->version);
    free(f->tt_version);
    free(f->notice);
    free(f->full_name);
    free(f->family_name);
    free(f->weight);
    free(f->italic_angle);
    free(f->font_name);

    TT_Close_Face(f->face);

    return;
}
