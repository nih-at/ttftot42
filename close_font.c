#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include <ftxpost.h>
#include <ftxerr18.h>

#include "t42.h"

void
close_font(font *f)
{
    free(f->version);
    free(f->tt_version);
    free(f->notice);
    free(f->full_name);
    free(f->family_name);
    free(f->weight);
    free(f->italic_angle);
    free(f->font_name);

    TT_Close_Face(f->face);

    return;
}
