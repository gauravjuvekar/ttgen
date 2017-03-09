EXTRA_DIST             = gui.ui LICENSE
bin_PROGRAMS           = project
nodist_project_SOURCES = gui.ui.hex.inc database.sqlite3.hex.inc LICENSE.hex.inc
BUILT_SOURCES          = gui.ui.hex.inc database.sqlite3.hex.inc LICENSE.hex.inc
project_SOURCES        = main.c\
                         notebook/notebook.c\
                         notebook/rooms.c       db_tables/rooms.c\
                         notebook/subjects.c    db_tables/subjects.c\
                         notebook/teachers.c    db_tables/teachers.c\
                         notebook/batches.c     db_tables/batches.c\
                         notebook/allocations.c db_tables/allocations.c\
                         notebook/schedules.c\
                         db_tables/population.c db_tables/schedule.c\
                         db_tables/meta.c       db_tables/db.c\
                         menubar/prefs.c        menubar/file.c\
                         menubar/help.c\
                         core/genetic.c core/evolution.c\
                         core/interface.c\
                         table_view/table.c
project_CPPFLAGS       = $(GTK3_CFLAGS)
project_LDADD          = $(GTK3_LIBS) -ldl -lpthread
project_LDFLAGS        = $(LTLIBINTL)
localedir              = $(datadir)/locale
DEFS                   = -DLOCALEDIR=\"$(localedir)\" @DEFS@
if !USE_SQLITE3
project_LDADD         += sqlite/libsqlite3.a
SUBDIRS                = sqlite
else
project_CPPFLAGS      += $(sqlite3_CFLAGS)
project_LDADD         += $(sqlite3_LIBS)
endif

CLEANFILES             = gui.ui.hex.inc database.sqlite3.hex.inc

SUFFIXES = .ui .ui.hex.inc .hex.inc

gui.ui.hex.inc : gui.ui
	xxd -i < $< > $@

database.sqlite3.hex.inc : database.sqlite3
	xxd -i < $< > $@

LICENSE.hex.inc : LICENSE
	xxd -i < $< > $@

