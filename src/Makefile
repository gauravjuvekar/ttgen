CC=gcc
CFLAGS+=-Wall -Wextra -std=c99 -O2
CFLAGS+=$(shell pkg-config --cflags gtk+-3.0)
LDLIBS=$(shell pkg-config --libs gtk+-3.0)

sqlite_dir=sqlite
sqlite_objects=$(sqlite_dir)/libsqlite3.a
LDLIBS+=-ldl -lpthread

TARGET=project

objects=\
	main.o\
	notebook/notebook.o\
	notebook/rooms.o\
	notebook/subjects.o\
	notebook/teachers.o\
	notebook/batches.o\
	notebook/allocations.o\
	notebook/schedules.o\
	db_tables/teachers.o\
	db_tables/batches.o\
	db_tables/rooms.o\
	db_tables/subjects.o\
	db_tables/allocations.o\
	db_tables/population.o\
	db_tables/schedule.o\
	db_tables/meta.o\
	db_tables/db.o\
	menubar/prefs.o\
	menubar/file.o\
	menubar/help.o\
	core/genetic.o\
	core/evolution.o\
	core/interface.o\
	table_view/table.o

hex_objects=$(addsuffix .hex.inc, gui.ui LICENSE database.sqlite3)

$(TARGET): $(objects) $(sqlite_objects) $(hex_objects)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(objects) $(sqlite_objects) $(LDLIBS)

gui.ui.hex.inc : gui.ui
	xxd -i < $< > $@

LICENSE.hex.inc : LICENSE
	xxd -i < $< > $@

database.sqlite3.hex.inc : database.sqlite3
	xxd -i < $< > $@

$(sqlite_objects):
	$(MAKE) -C sqlite

%.o : %.c $(hex_objects)
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $< $(LDLIBS)

.PHONY: clean
clean:
	rm --force --recursive -- $(objects) $(hex_objects) $(TARGET)
	$(MAKE) -C $(sqlite_dir) clean
