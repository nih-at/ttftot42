This is `ttftot42`, a program to facilitate using TrueType fonts on
PostScript interpreters with TrueType rasterizer.

You need the [FreeType](http://www.freetype.org/) library to compile and use it.

TrueType fonts cannot be converted to Type 1 fonts without loss of
quality, since conversion introduces rounding errors and loses hinting
information.  Therefore, newer PostScript interpreters (Adobe
PostScript since version 2013, ghostscript since 5.03) include a
TrueType rasterizers.  To use a TrueType font with them, it has to be
embedded in a PostScript font dictionary; this format is called Type 42.

This program generates such Type 42 fonts from TrueType fonts.  It
can also generate Adobe Font Metrics files (AFM files), including
kerning information.

Support for the creation of CID keyed fonts from large TrueType
fonts (East Asian or Unicode fonts) is included in this version, but
it is far from finished: AFM files are not created and glyph
substitution for vertical writing mode and alternative glyph forms
(e. g. localized versions of kanji) is not done, since support for it
is not implemented in FreeType 1.2.  Thus, the resulting fonts cannot
be used in vertical writing mode.

Not yet supported are TrueType Collections.

If you make a binary distribution, please include a pointer to the
distribution site:
	https://nih.at/ttftot42

The latest version can always be found there.

Mail suggestions and bug reports to <ttftot42@nih.at>.
