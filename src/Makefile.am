EXTRA_DIST             = gui.ui LICENSE database.sqlite3 \
                         libs.h                  \
                         main.h                  \
                         callback.h              \
                         notebook/notebook.h     \
                         notebook/rooms.h        \
                         db_tables/rooms.h       \
                         notebook/subjects.h     \
                         db_tables/subjects.h    \
                         notebook/teachers.h     \
                         db_tables/teachers.h    \
                         notebook/batches.h      \
                         db_tables/batches.h     \
                         notebook/allocations.h  \
                         db_tables/allocations.h \
                         notebook/schedules.h    \
                         db_tables/population.h  \
                         db_tables/schedule.h    \
                         db_tables/meta.h        \
                         db_tables/db.h          \
                         menubar/prefs.h         \
                         menubar/file.h          \
                         menubar/help.h          \
                         core/genetic.h          \
                         core/evolution.h        \
                         core/interface.h        \
                         table_view/table.h
bin_PROGRAMS           = project
nodist_project_SOURCES = gui.ui.hex.inc database.sqlite3.hex.inc LICENSE.hex.inc
BUILT_SOURCES          = gui.ui.hex.inc database.sqlite3.hex.inc LICENSE.hex.inc
project_SOURCES        = main.c                  \
                         notebook/notebook.c     \
                         notebook/rooms.c        \
                         db_tables/rooms.c       \
                         notebook/subjects.c     \
                         db_tables/subjects.c    \
                         notebook/teachers.c     \
                         db_tables/teachers.c    \
                         notebook/batches.c      \
                         db_tables/batches.c     \
                         notebook/allocations.c  \
                         db_tables/allocations.c \
                         notebook/schedules.c    \
                         db_tables/population.c  \
                         db_tables/schedule.c    \
                         db_tables/meta.c        \
                         db_tables/db.c          \
                         menubar/prefs.c         \
                         menubar/file.c          \
                         menubar/help.c          \
                         core/genetic.c          \
                         core/evolution.c        \
                         core/interface.c        \
                         table_view/table.c
project_CPPFLAGS       = $(GTK3_CFLAGS)
project_LDADD          = $(GTK3_LIBS)
project_LDFLAGS        = $(LTLIBINTL)
project_CPPFLAGS      += -DLOCALEDIR=\"$(localedir)\"
if !USE_SQLITE3
project_LDADD         += sqlite/libsqlite3.la
project_LDFLAGS       += -static
SUBDIRS                = sqlite
else
project_CPPFLAGS      += $(sqlite3_CFLAGS)
project_LDADD         += $(sqlite3_LIBS)
endif
project_LDADD         += -ldl -lpthread

CLEANFILES             = gui.ui.hex.inc database.sqlite3.hex.inc

SUFFIXES = .ui .ui.hex.inc .hex.inc

gui.ui.hex.inc : gui.ui
	xxd -i < $< > $@

database.sqlite3.hex.inc : database.sqlite3
	xxd -i < $< > $@

LICENSE.hex.inc : LICENSE
	xxd -i < $< > $@

