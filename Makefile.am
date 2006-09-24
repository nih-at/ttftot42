LDADD=@LIBOBJS@ ${FT2_LIBS}
DEFS = @DEFS@ -DDATADIR=\"$(pkgdatadir)\" -I. -I$(srcdir) -I.
AM_CFLAGS=${FT2_CFLAGS}

SUBDIRS=. cid

bin_PROGRAMS=ttftot42 acidtocid cidtoacid

ttftot42_SOURCES=main.c init.c open_font.c close_font.c get_name.c util.c \
	write_t42.c write_afm.c write_cid2.c xmalloc.c \
	cid_mkmap.c cid_read.c cid_util.c cid_decode.c \
	encodings.c substext.c 

acidtocid_SOURCES=acidtocid.c cid_aread.c cid_write.c cid_util.c pideid.c \
	xmalloc.c substext.c

cidtoacid_SOURCES=cidtoacid.c cid_read.c cid_awrite.c cid_util.c cid_decode.c \
	pideid.c xmalloc.c substext.c 

noinst_HEADERS=t42.h cid.h pideid.h substext.h getopt.h

EXTRA_DIST=REFERENCES ttftot42.lsm ttftot42.spec cid-format mkfontmap
