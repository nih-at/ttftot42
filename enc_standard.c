#define NDEF ".notdef"

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
