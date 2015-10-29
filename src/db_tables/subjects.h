#ifndef DB_TABLES_SUBJECTS_H
#define DB_TABLES_SUBJECTS_H 1

#include <glib.h>
#include "../sqlite/sqlite3.h"


typedef struct Subject {
	gint        pk;
	const gchar *name;
} Subject;


Subject Subject_from_stmt(sqlite3_stmt *stmt);
void    insert_Subject(sqlite3 *db, const Subject *subject);

#endif
