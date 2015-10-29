#ifndef DB_TABLES_ROOMS_H
#define DB_TABLES_ROOMS_H 1

#include <glib.h>
#include "../sqlite/sqlite3.h"


typedef struct Room {
	gint        pk;
	const gchar *name;
	gint        capacity;
	gint        parallel;
} Room;


Room Room_from_stmt(sqlite3_stmt *stmt);

#endif
