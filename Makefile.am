AUTOMAKE_OPTIONS=foreign

LDADD=@LIBOBJS@ ${FT2_LIBS}
DEFS = @DEFS@ -DDATADIR=\"$(pkgdatadir)\" -I. -I$(srcdir) -I.
AM_CFLAGS=${FT2_CFLAGS}

SUBDIRS=. cid

bin_PROGRAMS=ttftot42 acidtocid cidtoacid

ttftot42_SOURCES=\
	cid_decode.c \
	cid_mkmap.c \
	cid_read.c \
	cid_util.c \
	close_font.c \
	encodings.c \
	get_name.c \
	init.c \
	main.c \
	open_font.c \
	substext.c \
	util.c \
	write_afm.c \
	write_cid2.c \
	write_t42.c \
	xmalloc.c

acidtocid_SOURCES=\
	acidtocid.c \
	cid_aread.c \
	cid_util.c \
	cid_write.c \
	pideid.c \
	substext.c \
	xmalloc.c

cidtoacid_SOURCES=\
	cid_awrite.c \
	cid_decode.c \
	cid_read.c \
	cid_util.c \
	cidtoacid.c \
	pideid.c \
	substext.c \
	xmalloc.c

noinst_HEADERS=\
	cid.h \
	getopt.h \
	pideid.h \
	substext.h \
	t42.h

EXTRA_DIST=\
	REFERENCES \
	cid-format \
	mkfontmap \
	ttftot42.lsm \
	ttftot42.spec
