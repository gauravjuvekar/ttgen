EXTRA_DIST = gui.ui
bin_PROGRAMS =  project
BUILT_SOURCES = gui.ui.hex.inc
project_SOURCES = main.c
nodist_project_SOURCES = gui.ui.hex.inc
CLEANFILES = gui.ui.hex.inc
project_CPPFLAGS = $(GTK3_CFLAGS)
project_LDADD    = $(GTK3_LIBS)

SUFFIXES = .ui .ui.hex.inc .hex.inc

gui.ui.hex.inc : gui.ui
	xxd -i < $< > $@



