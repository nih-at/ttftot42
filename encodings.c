/*
  enc_standard -- Adobe Standard, ISO Latin 1, and PDF Doc Encodings
  Copyright (C) 1998 Dieter Baron

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
    "acute", "mu", "paragraph", "perodcentered",
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
    { "std", "StandardEncoding", 1, enc_standard, NULL },
    { "latin1", "ISOLatin1Encoding", 0, enc_latin1, NULL },
    { "pdf", "PDFDocEncoding", 0, enc_pdf, NULL },
};

int nencoding = sizeof(encoding)/sizeof(encoding[0]);
