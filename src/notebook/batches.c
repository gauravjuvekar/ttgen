#include "../main.h"

#include "batches.h"
#include "../db_tables/batches.h"

typedef enum {
	COLUMN_INT_pk,
	COLUMN_STRING_name,
	COLUMN_INT_heads,
	COLUMN_INT_parent,
	N_COLUMNS
} TreeView_Batches_E;

static GtkListStore *Batches_ListStore_new(void);
static void set_Batches_from_db(GtkListStore *list_store, sqlite3 *db);

static GtkListStore *Batches_ListStore_new(void) {
	return gtk_list_store_new(N_COLUMNS,
	                          G_TYPE_INT,
	                          G_TYPE_STRING,
	                          G_TYPE_INT,
	                          G_TYPE_INT);
}

void init_notebook_batches(CallBackData *data) {
	GtkTreeView *batches_tree_view = GTK_TREE_VIEW(
		gtk_builder_get_object(data->builder, "batches_tree_view")
	);
	GtkListStore *list_store = Batches_ListStore_new();
	set_Batches_from_db(list_store, data->db);
	gtk_tree_view_set_model(batches_tree_view, GTK_TREE_MODEL(list_store));

	gtk_tree_view_append_column(
		batches_tree_view,
		gtk_tree_view_column_new_with_attributes(
			"Name",
			gtk_cell_renderer_text_new(), "text", COLUMN_STRING_name, NULL)
	);
	gtk_tree_view_append_column(
		batches_tree_view,
		gtk_tree_view_column_new_with_attributes(
			"Heads",
			gtk_cell_renderer_text_new(), "text", COLUMN_INT_heads, NULL)
	);
	gtk_tree_view_append_column(
		batches_tree_view,
		gtk_tree_view_column_new_with_attributes(
			"Parent",
			gtk_cell_renderer_text_new(), "text", COLUMN_INT_parent, NULL)
	);


	GObject *batches_parent_combobox = gtk_builder_get_object(
		data->builder, "batches_add_window_parent_combobox");
	gtk_combo_box_set_model((GtkComboBox *)batches_parent_combobox,
	                        GTK_TREE_MODEL(list_store));

	GtkCellRenderer *combobox_text = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(batches_parent_combobox),
	                           combobox_text, TRUE);
	gtk_cell_layout_add_attribute(GTK_CELL_LAYOUT(batches_parent_combobox),
	                              combobox_text,
	                              "text", COLUMN_STRING_name);
}

static void set_Batches_from_db(GtkListStore *list_store, sqlite3 *db) {
	sqlite3_stmt *stmt;
	sqlite3_prepare(db, "SELECT * FROM batches", -1, &stmt, NULL);
	GtkTreeIter iter;
	while(sqlite3_step(stmt) == SQLITE_ROW) {
		gtk_list_store_append(list_store, &iter);
		Batch batch = Batch_from_stmt(stmt);
		gtk_list_store_set(list_store, &iter,
		                   COLUMN_INT_pk,      batch.pk,
		                   COLUMN_STRING_name, batch.name,
		                   COLUMN_INT_heads,   batch.heads,
		                   COLUMN_INT_parent,  batch.parent,
		                   -1);
	}
	sqlite3_finalize(stmt);
}

void refresh_notebook_batches(CallBackData *data) {
	GtkTreeView *batches_tree_view = GTK_TREE_VIEW(
		gtk_builder_get_object(data->builder, "batches_tree_view")
	);
	GtkTreeModel *list_store = gtk_tree_view_get_model(batches_tree_view);
	gtk_list_store_clear((GtkListStore *)list_store);
	set_Batches_from_db((GtkListStore *)list_store, data->db);
}
