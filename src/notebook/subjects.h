#ifndef NOTEBOOK_SUBJECTS_H
#define NOTEBOOK_SUBJECTS_H 1

#include <gtk/gtk.h>
#include "../sqlite/sqlite3.h"


void init_notebook_subjects(GtkBuilder *builder, sqlite3 *db);

#endif
