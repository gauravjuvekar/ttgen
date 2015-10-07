EXTRA_DIST             = gui.ui
bin_PROGRAMS           = project
nodist_project_SOURCES = gui.ui.hex.inc
BUILT_SOURCES          = gui.ui.hex.inc
project_SOURCES        = main.c\
                         notebook/rooms.c    db_tables/rooms.c\
                         notebook/subjects.c db_tables/subjects.c
project_CPPFLAGS       = $(GTK3_CFLAGS)
project_LDADD          = $(GTK3_LIBS) sqlite/libsqlite3.a -ldl -lpthread
SUBDIRS                = sqlite

CLEANFILES             = gui.ui.hex.inc

SUFFIXES = .ui .ui.hex.inc .hex.inc

gui.ui.hex.inc : gui.ui
	xxd -i < $< > $@



