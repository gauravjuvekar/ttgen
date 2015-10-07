#ifndef NOTEBOOK_TEACHERS_H
#define NOTEBOOK_TEACHERS_H 1

#include <gtk/gtk.h>
#include "../sqlite/sqlite3.h"


void init_notebook_teachers(GtkBuilder *builder, sqlite3 *db);

#endif
