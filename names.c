#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <freetype.h>

char *name[] = {
    "Copyright:    ",
    "Family:       ",
    "Subfamily:    ",
    "Unique-ID:    ",
    "Full-Name:    ",
    "Version:      ",
    "PS-Name:      ",
    "Trademark:    ",
    "Manufacturer: ",
    "Designer:     ",
    "Description:  ",
    "Vendor-Url:   ",
    "Designer-Url: "
};

int nname = sizeof(name)/sizeof(name[0]);

TT_Engine fte;



int print_names(char *fname);
char *get_name(TT_Face f, int nnames, int name);



int
main(int argc, char **argv)
{
    int i;

    TT_Init_FreeType(&fte);

    for (i=1; i<argc; i++)
	print_names(argv[i]);

    TT_Done_FreeType(fte);
    
    exit(0);
}



int
print_names(char *fname)
{
    char *s;
    int i;
    short pl, en;

    TT_Face face;
    TT_Face_Properties prop;

    TT_Open_Face(fte, fname, &face);
    TT_Get_Face_Properties(face, &prop);

    printf("File:         %s\n", fname);
    for (i=0; i<nname; i++) {
	s = get_name(face, prop.num_Names, i);
	if (s)
	    printf("%s%s\n", name[i], s);
    }
    printf("Vendor-ID:    %.4s\n", prop.os2->achVendID);

    printf("CMaps:        ");
    for (i=0; i<TT_Get_CharMap_Count(face); i++) {
	TT_Get_CharMap_ID(face, i, &pl, &en);
	if (i != 0)
	    printf(", ");
	printf("%d/%d", (int)pl, (int)en);
    }
    putchar('\n');

    putchar('\n');

    TT_Close_Face(face);

    return 0;
}
