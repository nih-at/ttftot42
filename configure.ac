AC_INIT([ttftot42], [0.3d+], [nih@giga.or.at])
AC_CONFIG_SRCDIR([write_t42.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign])

AC_PROG_CC

AC_CHECK_FT2
if test "$no_ft" = yes
then
    AC_MSG_ERROR([freetype is required for this program])
fi

AC_CHECK_HEADERS(libgen.h)

AC_CHECK_FUNCS(TTO_GSUB_Apply)

AC_REPLACE_FUNCS(strdup strerror basename)
AC_CHECK_FUNCS(getopt_long, , [LIBOBJS="$LIBOBJS getopt1.o getopt.o"])

AC_OUTPUT(Makefile cid/Makefile)
