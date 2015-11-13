#ifndef DB_TABLES_ROOMS_H
#define DB_TABLES_ROOMS_H 1

#include "../libs.h"

typedef struct Room {
	gint        pk;
	const gchar *name;
	gint        capacity;
	gint        parallel;
} Room;


Room Room_from_stmt(sqlite3_stmt *stmt);
void insert_Room(sqlite3 *db, const Room *room);
void remove_Room(sqlite3 *db, gint pk);

#endif
