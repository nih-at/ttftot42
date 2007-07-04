/*
  pideid.c -- convert Platform ID and Encoding ID between string and int
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



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int as_int(char *s);



/* data taken from FreeType 1.2 <ftnameid.h> */

static char *pidname[] = {
    "Apple-Unicode",
    "Macintosh",
    "ISO",
    "Microsoft"
};
static int npidname = sizeof(pidname)/sizeof(pidname[0]);

static char *eidapple[] = {
    "Default",
    "Unicode-1.1",
    "ISO-10646",
    "Unicode-2.0"
};

static char *eidmac[] = {
    "Roman",
    "Japanese",
    "Traditional-Chinese",
    "Korean",
    "Arabic",
    "Hebrew",
    "Greek",
    "Russian",
    "Rsymbol",
    "Devanagari",
    "Gurmukhi",
    "Gujarati",
    "Oriya",
    "Bengali",
    "Tamil",
    "Telugu",
    "Kannada",
    "Malayalam",
    "Sinhalese",
    "Burmese",
    "Khmer",
    "Thai",
    "Laotian",
    "Georgian",
    "Armenian",
    "Maldivian",
    "Simplified-Chinese",
    "Tibetan",
    "Mongolian",
    "Geez",
    "Slavic",
    "Vietnamese",
    "Sindhi",
    "Uninterp",
};

static char *eidiso[] = {
    "7bit-ASCII",
    "10646",
    "8859-1",
};

static char *eidms[] = {
    "Symbol",
    "Unicode",
    "SJIS",
    "GB2312",
    "Big 5",
    "Wansung",
    "Johab"
};

static char **eidname[] = {
    eidapple,
    eidmac,
    eidiso,
    eidms
};
static int neidname[] = {
    sizeof(eidapple)/sizeof(eidapple[0]),
    sizeof(eidmac)/sizeof(eidmac[0]),
    sizeof(eidiso)/sizeof(eidiso[0]),
    sizeof(eidms)/sizeof(eidms[0]),
};



int
otf_str2pid(char *s)
{
    int i;

    for (i=0; i<npidname; i++)
	if (strcasecmp(pidname[i], s) == 0)
	    return i;

    return as_int(s);
}



int
otf_str2eid(int pid, char *s)
{
    int i;
    
    if (pid < 0 || pid > npidname)
	return as_int(s);

    for (i=0; i<neidname[pid]; i++)
	if (strcasecmp(eidname[pid][i], s) == 0)
	    return i;
    
    return as_int(s);
}



char *
otf_pid2str(int pid)
{
    static char b[80];
    
    if (pid < 0 || pid > npidname) {
	sprintf(b, "%d", pid);
	return b;
    }

    return pidname[pid];
}



char *
otf_eid2str(int pid, int eid)
{
    static char b[80];
    
    if ((pid < 0 || pid > npidname
	 || eid < 0 || eid > neidname[pid])) {
	sprintf(b, "%d", eid);
	return b;
    }

    return eidname[pid][eid];
}



static int
as_int(char *s)
{
    int i;
    char *q;

    i = strtol(s, &q, 10);
    if ((i == 0 && *s != '0') || *q != '\0')
	return -1;
    return i;
}
