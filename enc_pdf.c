/*
  enc_pdf.c -- PDF Doc Encoding
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



#define NDEF ".notdef"

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
