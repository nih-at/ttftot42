#include <ftxpost.h>
#include <ftxkern.h>

#include "t42.h"

TT_Engine fte;

int
init(void)
{
    TT_Init_FreeType(&fte);
    TT_Init_Post_Extension(fte);
    TT_Init_Kerning_Extension(fte);

    return 0;
}
