FREETYPE=$(HOME)/ftp/type/freetype-1.2

CFLAGS=-I. -I/usr/local/include -g -Wall -I$(FREETYPE) -I$(FREETYPE)/lib
LDFLAGS=-L/usr/local/lib
LOADLIBES=-lttf

OBJS=main.o open_font.o write_t42.o write_afm.o init.o get_name.o \
	enc_standard.o \
	getopt.o getopt1.o ftxtbls.o

all: otftot42

otftot42: $(OBJS)
	$(CC) $(LDFLAGS) -o otftot42 $(OBJS) $(LOADLIBES)

main.o: main.c t42.h
open_font.o: open_font.c t42.h
write_t42.o: write_t42.c t42.h
init.o: init.c t42.h
get_name.o: get_name.c t42.h
