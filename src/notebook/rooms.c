#include <gtk/gtk.h>

#include "rooms.h"
#include "../sqlite/sqlite3.h"
#include "../db_tables/rooms.h"


GtkListStore *Rooms_ListStore_new(void) {
	return gtk_list_store_new(N_COLUMNS,
	                          G_TYPE_STRING,
	                          G_TYPE_INT,
	                          G_TYPE_INT);
}

void set_Rooms_from_db(GtkListStore *list_store, sqlite3 *db) {
	sqlite3_stmt *stmt;
	sqlite3_prepare(db, "SELECT * FROM rooms", -1, &stmt, NULL);
	GtkTreeIter iter;
	while(sqlite3_step(stmt) == SQLITE_ROW) {
		gtk_list_store_append(list_store, &iter);
		Room room = Room_from_stmt(stmt);
		gtk_list_store_set(list_store, &iter,
		                   COLUMN_STRING_name,  room.name,
		                   COLUMN_INT_capacity, room.capacity,
		                   COLUMN_INT_parallel, room.parallel,
		                   -1);
	}
	sqlite3_finalize(stmt);
}
