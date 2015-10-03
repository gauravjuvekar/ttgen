#include <gtk/gtk.h>

#include "rooms.h"


GtkListStore *Rooms_ListStore_new(void) {
	return gtk_list_store_new(N_COLUMNS,
	                          G_TYPE_STRING,
	                          G_TYPE_INT,
	                          G_TYPE_INT);
}

void set_Rooms_from_db(GtkListStore *list_store) {
	GtkTreeIter iter;

	/* TODO FIXME */

	gint i;
	for(i = 0; i < 10; i++) {
		gtk_list_store_append(list_store, &iter);
		gtk_list_store_set(list_store, &iter,
		                   COLUMN_STRING_name, "Some name",
		                   COLUMN_INT_capacity, 70,
		                   COLUMN_INT_parallel, 1,
		                   -1);
	}
}
