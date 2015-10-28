#ifndef NOTEBOOK_BATCHES_H
#define NOTEBOOK_BATCHES_H 1

#include <gtk/gtk.h>
#include "../sqlite/sqlite3.h"


void init_notebook_batches(GtkBuilder *builder, sqlite3 *db);

#endif
