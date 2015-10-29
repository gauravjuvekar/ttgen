#ifndef NOTEBOOK_BATCHES_H
#define NOTEBOOK_BATCHES_H 1


#include "../main.h"

void init_notebook_batches(GtkBuilder *builder, sqlite3 *db);
void refresh_notebook_batches(GtkBuilder* builder, sqlite3 *db);

#endif
