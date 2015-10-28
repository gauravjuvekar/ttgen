#ifndef DB_TABLES_BATCHES_H
#define DB_TABLES_BATCHES_H 1

#include <glib.h>
#include "../sqlite/sqlite3.h"


typedef struct Batch {
	gint        pk;
	const gchar *name;
	gint        heads;
	gint        parent;
} Batch;


Batch Batch_from_stmt(sqlite3_stmt *stmt);

#endif
