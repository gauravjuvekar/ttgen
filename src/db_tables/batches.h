#ifndef DB_TABLES_BATCHES_H
#define DB_TABLES_BATCHES_H 1

#include <gtk/gtk.h>
#include <glib.h>
#include "../sqlite/sqlite3.h"


typedef struct Batch {
	gint        pk;
	const gchar *name;
	gint        heads;
	gint        parent;
} Batch;


Batch Batch_from_stmt(sqlite3_stmt *stmt);
void  refresh_notebook_batches(GtkBuilder* builder, sqlite3 *db);

#endif
