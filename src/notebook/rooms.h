#ifndef NOTEBOOK_ROOMS_H
#define NOTEBOOK_ROOMS_H 1

#include <gtk/gtk.h>
#include "../sqlite/sqlite3.h"


void init_notebook_rooms(GtkBuilder *builder, sqlite3 *db);

#endif
