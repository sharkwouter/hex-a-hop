AC_PREREQ([2.58])
AC_INIT([Hex-a-hop],[1.2.0],[http://hexahop.sourceforge.net],[hex-a-hop])
AC_CONFIG_AUX_DIR([msc])
AC_CONFIG_MACRO_DIR([msc])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_HEADERS([src/config.h])
AM_MAINTAINER_MODE([enable])

AC_SEARCH_LIBS([strerror],[cposix])
AC_PROG_CC
AC_PROG_CXX

AC_HEADER_STDC

AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.18])

AC_CHECK_HEADERS(stdint.h windows.h)
AC_CHECK_LIB(m, main)

AM_ICONV
if test "$am_cv_func_iconv" = yes; then
       AC_DEFINE_UNQUOTED(ICONV_CONST, [$am_cv_proto_iconv_arg1],
               [Define as const if the declaration of iconv() needs const.])
fi

# Check for SDL.
SDL2_VERSION=2.0.0
AM_PATH_SDL($SDL2_VERSION, :, AC_MSG_ERROR([*** SDL version $SDL2_VERSION not found!]))

# Arguments.
AC_ARG_ENABLE([debug], AS_HELP_STRING([--disable-debug],[Do not compile with debugging symbols]), [want_debug=$enableval], [want_debug=no])
AC_ARG_ENABLE([sound], AS_HELP_STRING([--disable-sound],[Do not compile with sound support]), [want_sound=$enableval], [want_sound=yes])
AC_ARG_ENABLE([relpath], AS_HELP_STRING([--enable-relpath],[Install everything under a single directory]), [want_relative=$enableval], [want_relative=no])

# Allow specifiying a hard-coded font file at build time
AC_ARG_WITH(font-path,
        AS_HELP_STRING([--with-font-path=file.ttf],
                [Path to the TTF font to use at runtime with SDL2_ttf (DejaVu Sans Bold recommended)]),
        [FONT_PATH="$withval"],
        [FONT_PATH=""])

# Compilation flags.
if test "$want_debug" = yes; then
	DEBUG_FLAGS="-g -D_DEBUG"
else
	DEBUG_FLAGS="-DNDEBUG"
fi

# Check for SDL_image.
SDL_LIBS="$SDL_LIBS -lSDL2_image"
SDL_CFLAGS="$SDL_CFLAGS"

# Conditional sound compilation.
if test "$want_sound" = yes; then
	MAYBE_SOUND="sound"

	# Check for SDL_mixer.
	SDL_LIBS="$SDL_LIBS -lSDL2_mixer"
	SDL_CFLAGS="$SDL_CFLAGS"
else
	MAYBE_SOUND=
	SOUND_FLAGS="-DDISABLE_SOUND"
fi

# Check for SDL_ttf.
SDL_LIBS="$SDL_LIBS -lSDL2_ttf"

# Font path
if test "x$FONT_PATH" != x ; then
        TEXT_FLAGS="$TEXT_FLAGS -DFONT_PATH=\"\\\"$FONT_PATH\\\"\""
else
	AC_MSG_ERROR([Font path not configured, please use the --with-font-path configure argument])
fi

# Relative paths.
if test "$want_relative" = yes; then
	APPDIR="${prefix}/data"
	DATADIR="${prefix}/data"
	bindir="${prefix}"
	docdir="${prefix}/data"
	htmldir="${prefix}/data"
	mandir="${prefix}/data"
	PATH_CPPFLAGS="-DRELATIVE_PATHS"
	PATH_LDFLAGS="-Wl,-rpath -Wl,\\\$\$ORIGIN/lib"
else
	APPDIR="${datarootdir}/applications"
	DATADIR="${datadir}/${PACKAGE}"
	PATH_CPPFLAGS="-DDATADIR=\\\"\"$DATADIR\"\\\" "-DLOCALEDIR=\\\"\"$localedir\"\\\"
fi

# Export configuration.
AC_SUBST([DEBUG_FLAGS])
AC_SUBST([TEXT_FLAGS])
AC_SUBST([SOUND_FLAGS])
AC_SUBST([PATH_CPPFLAGS])
AC_SUBST([PATH_LDFLAGS])
AC_SUBST([APPDIR])
AC_SUBST([DATADIR])
AC_SUBST([MAYBE_SOUND])
AC_SUBST([SDL_LIBS])
AC_SUBST([SDL_CFLAGS])

AC_CONFIG_FILES([
Makefile
data/Makefile
src/Makefile
po/Makefile.in
])
AC_OUTPUT
