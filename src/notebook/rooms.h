#ifndef NOTEBOOK_ROOMS_H
#define NOTEBOOK_ROOMS_H 1

#include <gtk/gtk.h>


typedef enum {
	COLUMN_STRING_name,
	COLUMN_INT_capacity,
	COLUMN_INT_parallel,
	N_COLUMNS
} TreeView_Rooms_E;

GtkListStore *Rooms_ListStore_new(void);
void set_Rooms_from_db(GtkListStore *list_store);

#endif
