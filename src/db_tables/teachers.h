#ifndef DB_TABLES_TEACHERS_H
#define DB_TABLES_TEACHERS_H 1

#include <glib.h>
#include "../sqlite/sqlite3.h"


typedef struct Teacher {
	gint        pk;
	const gchar *name;
} Teacher;


Teacher Teacher_from_stmt(sqlite3_stmt *stmt);

#endif
