#ifndef MAIN_H
#define MAIN_H 1

#include <gtk/gtk.h>
#include <glib.h>
#include "sqlite/sqlite3.h"

typedef struct CallBackData {
	sqlite3    *db;
	GtkBuilder *builder;
} CallBackData;

#endif
