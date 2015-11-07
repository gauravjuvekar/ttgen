EXTRA_DIST             = gui.ui
bin_PROGRAMS           = project
nodist_project_SOURCES = gui.ui.hex.inc database.sqlite3.hex.inc
BUILT_SOURCES          = gui.ui.hex.inc database.sqlite3.hex.inc
project_SOURCES        = main.c\
                         notebook/notebook.c\
                         notebook/rooms.c       db_tables/rooms.c\
                         notebook/subjects.c    db_tables/subjects.c\
                         notebook/teachers.c    db_tables/teachers.c\
                         notebook/batches.c     db_tables/batches.c\
                         notebook/allocations.c db_tables/allocations.c\
                         db_tables/meta.c\
                         core/genetic.c core/evolution.c
project_CPPFLAGS       = $(GTK3_CFLAGS)
project_LDADD          = $(GTK3_LIBS) sqlite/libsqlite3.a -ldl -lpthread
SUBDIRS                = sqlite

CLEANFILES             = gui.ui.hex.inc database.sqlite3.hex.inc

SUFFIXES = .ui .ui.hex.inc .hex.inc

gui.ui.hex.inc : gui.ui
	xxd -i < $< > $@

database.sqlite3.hex.inc : database.sqlite3
	xxd -i < $< > $@




