/*
  cid_decode.c -- decode cid_code
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



#include "cid.h"



int
cid_decode_init(struct cid_code *code)
{
    code->cmd = C_none;
    code->idx = code->len = 0;

    return 0;
}



int
cid_decode(struct cid_code *code, int *value)
{
    int flags;

    if (code->len == 0) {
	if (code->idx >= code->ndata) {
	    return CID_NENC & CID_EOF;
	}
	
	code->cmd = code->data[code->idx] & 0xe000;
	code->len = code->data[code->idx] & 0x1fff;
	code->idx++;
	if (code->cmd == C_AM || code->cmd == C_O || code->cmd == C_OV)
	    code->state = code->data[code->idx++];
    }

    flags = 0;
    switch (code->cmd) {
    case C_N:
	flags = CID_NENC;
	break;
	
    case C_AM:
	*value = code->data[code->idx++];
	flags = ((code->state & 1 ? CID_VERT : 0)
		 | (code->len > 1 ? CID_ALT : 0));
	code->state >>= 1;
	break;

    case C_A:
    case C_AV:
	*value = code->data[code->idx++];
	flags = ((code->cmd == C_AV ? CID_VERT : 0)
		 | (code->len > 1 ? CID_ALT : 0));
	break;

    case C_L:
    case C_LV:
	*value = code->data[code->idx++];
	flags = code->cmd == C_LV ? CID_VERT : 0;
	break;

    case C_O:
    case C_OV:
	*value = code->state++;
	flags = code->cmd == C_OV ? CID_VERT : 0;
	break;
    }

    code->len -= 1;

    return flags;
}
