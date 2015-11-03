#ifndef DB_TABLES_BATCHES_H
#define DB_TABLES_BATCHES_H 1

#include "../main.h"

typedef struct Batch {
	gint        pk;
	const gchar *name;
	gint        heads;
	gint        parent;
} Batch;


Batch Batch_from_stmt(sqlite3_stmt *stmt);
void  insert_Batch(sqlite3 *db, const Batch *batch);
void  remove_Batch(sqlite3 *db, gint pk);

#endif
