CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c99 -O2

libsqlite3.a: sqlite.o
	ar cru $@ $<
	ranlib $@

sqlite.o: sqlite3.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm --force --recursive libsqlite3.a sqlite.o

