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
    putchar('\n');

    TT_Close_Face(face);

    return 0;
}



char *
get_name(TT_Face f, int nnames, int name)
{
    int i;
    TT_UShort pid, eid, lid, nid, len;
    char *p, *s, *t;

    for (i=0; i<nnames; i++) {
	TT_Get_Name_ID(f, i, &pid, &eid, &lid, &nid);

	if (nid == name) {
	    if (((pid == 1 /* mac */ && eid == 0 /* roman */)
		 || (pid == 3 /* ms */ && eid == 1 /* unicode */))) {
		TT_Get_Name_String(f, i, &p, &len);
		if (pid == 1) {
		    if ((s=(char *)malloc(len+1)) == NULL)
			return NULL;
		    strncpy(s, p, len);
		    s[len] = '\0';
		}
		else {
		    if ((s=(char *)malloc((len/2)+1)) == NULL)
			return NULL;
		    for (t=s; len; p+=2,len-=2) {
			if (*p == 0)
			    *(t++) = p[1];
		    }
		    *t = '\0';
		    return s;
		}
	    }
	}
    }

    return NULL;
}
