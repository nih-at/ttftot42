#include <stdio.h>
#include <stdlib.h>

#include "t42.h"

void *
xmalloc(size_t size)
{
    void *p;

    if ((p=malloc(size)) == NULL) {
	fprintf(stderr, "%s: malloc failure (size=%ld)\n",
		prg, (long)size);
	exit(1);
    }

    return p;
}
