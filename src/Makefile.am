EXTRA_DIST = config.h.cmake
MAINTAINERCLEANFILES = Makefile.in config.h.in config.h.in~

bin_PROGRAMS = \
	hex-a-hop

hex_a_hop_SOURCES = \
	gettext.h \
	gfx.cpp \
	gfx_list.h \
	hex_puzzzle.cpp \
	i18n.cpp \
	i18n.h \
	level_list.h \
	menus.h \
	packfile.h \
	savestate.h \
	sfx.cpp \
	sfx.h \
	state.h \
	system-directory.c \
	system-directory.h \
	system-relative.c \
	system-relative.h \
	text.cpp \
	text.h \
	tiletypes.h \
	video.h

hex_a_hop_CPPFLAGS = \
	$(DEBUG_FLAGS) \
	$(TEXT_FLAGS) \
	$(SOUND_FLAGS) \
	$(PATH_CPPFLAGS) \
	$(SDL2_CFLAGS)

hex_a_hop_LDFLAGS = \
	$(PATH_LDFLAGS)

hex_a_hop_LDADD = \
	$(SDL2_LIBS)
