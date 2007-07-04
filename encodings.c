/*
  encodings.c -- enconding vectors
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

#include "t42.h"

#define NDEF NULL



char *enc_standard[256] = {
    /* 0x00 */
    NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF,
    NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF,
    /* 0x10 */
    NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF,
    NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF,
    /* 0x20 */
    "space", "exclam", "quotedbl", "numbersign",
    "dollar", "percent", "ampersand", "quoteright",
    "parenleft", "parenright", "asterisk", "plus",
    "comma", "hyphen", "period", "slash",
    /* 0x30 */
    "zero", "one", "two", "three", "four", "five", "six", "seven",
    "eight", "nine", "colon", "semicolon",
    "less", "equal", "greater", "question",
    /* 0x40 */
    "at", "A", "B", "C", "D", "E", "F", "G",
    "H", "I", "J", "K", "L", "M", "N", "O",
    /* 0x50 */
    "P", "Q", "R", "S", "T", "U", "V", "W",
    "X", "Y", "Z", "bracketleft",
    "backslash", "bracketright", "asciicircum", "underscore",
    /* 0x60 */
    "quoteleft", "a", "b", "c", "d", "e", "f", "g",
    "h", "i", "j", "k", "l", "m", "n", "o",
    /* 0x70 */
    "p", "q", "r", "s", "t", "u", "v", "w",
    "x", "y", "z", "braceleft", "bar", "braceright", "asciitilde", NDEF,
    /* 0x80 */
    NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF,
    NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF,
    /* 0x90 */
    NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF,
    NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF,
    /* 0xa0 */
    NDEF, "exclamdown", "cent", "sterling",
    "fraction", "yen", "florin", "section",
    "currency", "quotesingle", "quotedblleft", "guillemotleft",
    "guilsinglleft", "guilsinglright", "fi", "fl",
    /* 0xb0 */
    NDEF, "endash", "dagger", "daggerdbl",
    "periodcentered", NDEF, "paragraph", "bullet",
    "quotesinglbase", "quotedblbase", "quotedblright", "guillemotright",
    "ellipsis", "perthousand", NDEF, "questiondown",
    /* 0xc0 */
    NDEF, "grave", "acute", "circumflex",
    "tilde", "macron", "breve", "dotaccent",
    "dieresis", NDEF, "ring", "cedilla",
    NDEF, "hungarumlaut", "ogonek", "caron",
    /* 0xd0 */
    "emdash", NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF,
    NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF,
    /* 0xe0 */
    NDEF, "AE", NDEF, "ordfeminine", NDEF, NDEF, NDEF, NDEF,
    "Lslash", "Oslash", "OE", "ordmasculine", NDEF, NDEF, NDEF, NDEF,
    /* 0xf0 */
    NDEF, "ae", NDEF, NDEF, NDEF, "dotlessi", NDEF, NDEF,
    "lslash", "oslash", "oe", "germandbls", NDEF, NDEF, NDEF, NDEF
};



char *enc_latin1[256] = {
    /* 0x00 */
    NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF,
    NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF,
    /* 0x10 */
    NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF,
    NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF,
    /* 0x20 */
    "space", "exclam", "quotedbl", "numbersign",
    "dollar", "percent", "ampersand", "quoteright",
    "parenleft", "parenright", "asterisk", "plus",
    "comma", "minus", "period", "slash",
    /* 0x30 */
    "zero", "one", "two", "three", "four", "five", "six", "seven",
    "eight", "nine", "colon", "semicolon",
    "less", "equal", "greater", "question",
    /* 0x40 */
    "at", "A", "B", "C", "D", "E", "F", "G",
    "H", "I", "J", "K", "L", "M", "N", "O",
    /* 0x50 */
    "P", "Q", "R", "S", "T", "U", "V", "W",
    "X", "Y", "Z", "bracketleft",
    "backslash", "bracketright", "asciicircum", "underscore",
    /* 0x60 */
    "quoteleft", "a", "b", "c", "d", "e", "f", "g",
    "h", "i", "j", "k", "l", "m", "n", "o",
    /* 0x70 */
    "p", "q", "r", "s", "t", "u", "v", "w",
    "x", "y", "z", "braceleft",
    "bar", "braceright", "asciitilde", NDEF,
    /* 0x80 */
    NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF,
    NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF,
    /* 0x90 */
    "dotlessi", "grave", "acute", "circumflex",
    "tilde", "macron", "breve", "dotaccent",
    "dieresis", NDEF, "ring", "cedilla",
    NDEF, "hungarumlaut", "ogonek", "caron",
    /* 0xa0 */
    "space", "exclamdown", "cent", "sterling",
    "currency", "yen", "brokenbar", "section",
    "dieresis", "copyright", "ordfeminine", "guillemotleft",
    "logicalnot", "hyphen", "registered", "macron",
    /* 0xb0 */
    "degree", "plusminus", "twosuperior", "threesuperior",
    "acute", "mu", "paragraph", "periodcentered",
    "cedilla", "onesuperior", "ordmasculine", "guillemotright",
    "onequarter", "onehalf", "threequarters", "questiondown",
    /* 0xc0 */
    "Agrave", "Aacute", "Acircumflex", "Atilde",
    "Adieresis", "Aring", "AE", "Ccedilla",
    "Egrave", "Eacute", "Ecircumflex", "Edieresis",
    "Igrave", "Iacute", "Icircumflex", "Idieresis",
    /* 0xd0 */
    "Eth", "Ntilde", "Ograve", "Oacute",
    "Ocircumflex", "Otilde", "Odieresis", "multiply",
    "Oslash", "Ugrave", "Uacute", "Ucircumflex",
    "Udieresis", "Yacute", "Thorn", "germandbls",
    /* 0xe0 */
    "agrave", "aacute", "acircumflex", "atilde",
    "adieresis", "aring", "ae", "ccedilla",
    "egrave", "eacute", "ecircumflex", "edieresis",
    "igrave", "iacute", "icircumflex", "idieresis",
    /* 0xf0 */
    "eth", "ntilde", "ograve", "oacute",
    "ocircumflex", "otilde", "odieresis", "divide",
    "oslash", "ugrave", "uacute", "ucircumflex",
    "udieresis", "yacute", "thorn", "ydieresis"
};



char *enc_1252[256] = {
    /* 0x00 */
    NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, 
    NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF,
    /* 0x10 */
    NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, 
    NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, 
    /* 0x20 */
    "space", "exclam", "quotedbl", "numbersign", 
    "dollar", "percent", "ampersand", "quotesingle", 
    "parenleft", "parenright", "asterisk", "plus", 
    "comma", "hyphen", "period", "slash", 
    /* 0x30 */
    "zero", "one", "two", "three", "four", "five", "six", "seven", 
    "eight", "nine", "colon", "semicolon",
    "less", "equal", "greater", "question", 
    /* 0x40 */
    "at", "A", "B", "C", "D", "E", "F", "G",
    "H", "I", "J", "K", "L", "M", "N", "O", 
    /* 0x50 */
    "P", "Q", "R", "S", "T", "U", "V", "W",
    "X", "Y", "Z", "bracketleft",
    "backslash", "bracketright", "asciicircum", "underscore", 
    /* 0x60 */
    "grave", "a", "b", "c", "d", "e", "f", "g",
    "h", "i", "j", "k", "l", "m", "n", "o", 
    /* 0x70 */
    "p", "q", "r", "s", "t", "u", "v", "w",
    "x", "y", "z", "braceleft", "bar", "braceright", "asciitilde", NDEF, 
    /* 0x80 */
    NDEF, NDEF, "quotesinglbase", "florin", 
    "quotedblbase", "ellipsis", "dagger", "daggerdbl", 
    "circumflex", "perthousand", "Scaron", "guilsinglleft",
    "OE", NDEF, NDEF, NDEF, 
    /* 0x90 */
    NDEF, "quoteleft", "quoteright", "quotedblleft", 
    "quotedblright", "bullet", "endash", "emdash", 
    "tilde", "trademark", "scaron", "guilsinglright", 
    "oe", NDEF, NDEF, "Ydieresis", 
    /* 0xa0 */
    NDEF, "exclamdown", "cent", "sterling", 
    "currency", "yen", "brokenbar", "section", 
    "dieresis", "copyright", "ordfeminine", "guillemotleft", 
    "logicalnot", "hyphen", "registered", "macron", 
    /* 0xb0 */
    "degree", "plusminus", "twosuperior", "threesuperior", 
    "acute", "mu", "paragraph", "periodcentered", 
    "cedilla", "onesuperior", "ordmasculine", "guillemotright", 
    "onequarter", "onehalf", "threequarters", "questiondown", 
    /* 0xc0 */
    "Agrave", "Aacute", "Acircumflex", "Atilde", 
    "Adieresis", "Aring", "AE", "Ccedilla", 
    "Egrave", "Eacute", "Ecircumflex", "Edieresis", 
    "Igrave", "Iacute", "Icircumflex", "Idieresis",
    /* 0xd0 */
    "Eth", "Ntilde", "Ograve", "Oacute", 
    "Ocircumflex", "Otilde", "Odieresis", "multiply", 
    "Oslash", "Ugrave", "Uacute", "Ucircumflex", 
    "Udieresis", "Yacute", "Thorn", "germandbls", 
    /* 0xe0 */
    "agrave", "aacute", "acircumflex", "atilde", 
    "adieresis", "aring", "ae", "ccedilla", 
    "egrave", "eacute", "ecircumflex", "edieresis", 
    "igrave", "iacute", "icircumflex", "idieresis", 
    /* 0xf0*/
    "eth", "ntilde", "ograve", "oacute", 
    "ocircumflex", "otilde", "odieresis", "divide", 
    "oslash", "ugrave", "uacute", "ucircumflex", 
    "udieresis", "yacute", "thorn", "ydieresis"
};

char *enc_pdf[256] = {
    /* 0x00 */
    NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF,
    NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF,
    /* 0x10 */
    NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF, NDEF,
    "breve", "caron", "circumflex", "dotaccent",
    "hungarumlaut", "ogonek", "ring", "tilde",
    /* 0x20 */
    "space", "exclam", "quotedbl", "numbersign",
    "dollar", "percent", "ampersand", "quotesingle",
    "parenleft", "parenright", "asterisk", "plus",
    "comma", "hyphen", "period", "slash",
    /* 0x30 */
    "zero", "one", "two", "three", "four", "five", "six", "seven",
    "eight", "nine", "colon", "semicolon",
    "less", "equal", "greater", "question",
    /* 0x40 */
    "at", "A", "B", "C", "D", "E", "F", "G",
    "H", "I", "J", "K", "L", "M", "N", "O",
    /* 0x50 */
    "P", "Q", "R", "S", "T", "U", "V", "W",
    "X", "Y", "Z", "bracketleft",
    "backslash", "bracketright", "asciicircum", "underscore",
    /* 0x60 */
    "grave", "a", "b", "c", "d", "e", "f", "g",
    "h", "i", "j", "k", "l", "m", "n", "o",
    /* 0x70 */
    "p", "q", "r", "s", "t", "u", "v", "w",
    "x", "y", "z", "braceleft",
    "bar", "braceright", "asciitilde", NDEF,
    /* 0x80 */
    "bullet", "dagger", "daggerdbl", "ellipsis",
    "emdash", "endash", "florin", "fraction",
    "guilsinglleft", "guilsinglright", "minus", "perthousand",
    "quotedblbase", "quotedblleft", "quotedblright", "quoteleft",
    /* 0x90 */
    "quoteright", "quotesinglbase", "trademark", "fi",
    "fl", "Lslash", "OE", "Scaron",
    "Ydieresis", "Zcaron", "dotlessi", "lslash",
    "oe", "scaron", "zcaron", NDEF,
    /* 0xA0 */
    NDEF, "exclamdown", "cent", "sterling",
    "currency", "yen", "brokenbar", "section",
    "dieresis", "copyright", "ordfeminine", "guillemotleft",
    "logicalnot", NDEF, "registered", "macron",
    /* 0xD0 */
    "degree", "plusminus", "twosuperior", "threesuperior",
    "acute", "mu", "paragraph", "periodcentered",
    "cedilla", "onesuperior", "ordmasculine", "guillemotright",
    "onequarter", "onehalf", "threequarters", "questiondown",
    /* 0xC0 */
    "Agrave", "Aacute", "Acircumflex", "Atilde",
    "Adieresis", "Aring", "AE", "Ccedilla",
    "Egrave", "Eacute", "Ecircumflex", "Edieresis",
    "Igrave", "Iacute", "Icircumflex", "Idieresis",
    /* 0xD0 */
    "Eth", "Ntilde", "Ograve", "Oacute",
    "Ocircumflex", "Otilde", "Odieresis", "multiply",
    "Oslash", "Ugrave", "Uacute", "Ucircumflex",
    "Udieresis", "Yacute", "Thorn", "germandbls",
    /* 0xE0 */
    "agrave", "aacute", "acircumflex", "atilde",
    "adieresis", "aring", "ae", "ccedilla",
    "egrave", "eacute", "ecircumflex", "edieresis",
    "igrave", "iacute", "icircumflex", "idieresis",
    /* 0xF0 */
    "eth", "ntilde", "ograve", "oacute",
    "ocircumflex", "otilde", "odieresis", "divide",
    "oslash", "ugrave", "uacute", "ucircumflex",
    "udieresis", "yacute", "thorn", "ydieresis",
};



struct encoding encoding[] = {
    /* first entry is default */
    { "std",    "StandardEncoding",      ENC_BUILTIN,  enc_standard, NULL, 0 },
    { "1252",   "Microsoft1252Encoding", 0,            enc_1252,     NULL, 0 },
    { "font",   "FontSpecificEncoding",  ENC_FROMFONT, NULL,         NULL, 0 },
    { "latin1", "ISOLatin1Encoding",     0,            enc_latin1,   NULL, 0 },
    { "pdf",    "PDFDocEncoding",        0,            enc_pdf,      NULL, 0 }
};

int nencoding = sizeof(encoding)/sizeof(encoding[0]);
