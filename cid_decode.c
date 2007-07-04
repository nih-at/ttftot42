/*
  cid_decode.c -- decode cid_code
  Copyright (C) 1999 Dieter Baron

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
