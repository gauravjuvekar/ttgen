#include <gtk/gtk.h>

#include "rooms.h"
#include "../sqlite/sqlite3.h"
#include "../db_tables/rooms.h"

typedef enum {
	COLUMN_STRING_name,
	COLUMN_INT_capacity,
	COLUMN_INT_parallel,
	N_COLUMNS
} TreeView_Rooms_E;

static GtkListStore *Rooms_ListStore_new(void);
static void set_Rooms_from_db(GtkListStore *list_store, sqlite3 *db);

static GtkListStore *Rooms_ListStore_new(void) {
	return gtk_list_store_new(N_COLUMNS,
	                          G_TYPE_STRING,
	                          G_TYPE_INT,
	                          G_TYPE_INT);
}

void init_notebook_rooms(GtkBuilder *builder, sqlite3 *db) {
	GtkTreeView *rooms_tree_view = GTK_TREE_VIEW(
		gtk_builder_get_object(builder, "rooms_tree_view")
	);
	GtkListStore *list_store = Rooms_ListStore_new();
	set_Rooms_from_db(list_store, db);
	gtk_tree_view_set_model(rooms_tree_view, GTK_TREE_MODEL(list_store));

	gtk_tree_view_append_column(
		rooms_tree_view,
		gtk_tree_view_column_new_with_attributes(
			"Name",
			gtk_cell_renderer_text_new(), "text", COLUMN_STRING_name, NULL)
	);
	gtk_tree_view_append_column(
		rooms_tree_view,
		gtk_tree_view_column_new_with_attributes(
			"Capacity",
			gtk_cell_renderer_text_new(), "text", COLUMN_INT_capacity, NULL)
	);
	gtk_tree_view_append_column(
		rooms_tree_view,
		gtk_tree_view_column_new_with_attributes(
			"Parallel",
			gtk_cell_renderer_text_new(), "text", COLUMN_INT_parallel, NULL)
	);

}

static void set_Rooms_from_db(GtkListStore *list_store, sqlite3 *db) {
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
