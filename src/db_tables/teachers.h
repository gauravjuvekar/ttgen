#ifndef DB_TABLES_TEACHERS_H
#define DB_TABLES_TEACHERS_H 1

#include "../main.h"


typedef struct Teacher {
	gint        pk;
	const gchar *name;
} Teacher;


Teacher Teacher_from_stmt(sqlite3_stmt *stmt);
void insert_Teacher(sqlite3 *db, const Teacher *teacher);

#endif
