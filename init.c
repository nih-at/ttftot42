/*
  init -- initialization (of FreeType et. al.)
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



#include "config.h"

#ifdef HAVE_FREETYPE_FREETYPE_H
#include <freetype/ftxpost.h>
#include <freetype/ftxkern.h>
#else
#include <ftxpost.h>
#include <ftxkern.h>
#endif

#include "t42.h"

TT_Engine fte;

int
init(void)
{
    TT_Init_FreeType(&fte);
    TT_Init_Post_Extension(fte);
    TT_Init_Kerning_Extension(fte);
#ifdef HAVE_GSUB
    TT_Init_GSUB_Extension(fte);
#endif

    return 0;
}



int
done(void)
{
    TT_Done_FreeType(fte);

    return 0;
}
