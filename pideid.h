#ifndef _HAD_PIDEID_H
#define _HAD_PIDEID_H

/*
  pideid.c -- convert Platform ID and Encoding ID between string and int
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



int otf_str2pid(char *s);
int otf_str2eid(int pid, char *s);
char *otf_pid2str(int pid);
char *otf_eid2str(int pid, int ed);

#endif /* pideid.h */
