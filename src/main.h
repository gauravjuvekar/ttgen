#ifndef MAIN_H
#define MAIN_H 1

#include <gtk/gtk.h>
#include <glib.h>
#include "sqlite/sqlite3.h"

typedef struct CallBackData {
	sqlite3    *db;
	GtkBuilder *builder;
	gchar      *db_name;
} CallBackData;

void quit_CB(GtkWidget *widget, CallBackData *data);
void init_all(CallBackData *data);

#endif
