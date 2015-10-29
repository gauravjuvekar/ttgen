#include "../main.h"

#include "teachers.h"
#include "../db_tables/teachers.h"

typedef enum {
	COLUMN_INT_pk,
	COLUMN_STRING_name,
	N_COLUMNS
} TreeView_Teachers_E;

static GtkListStore *Teachers_ListStore_new(void);
static void set_Teachers_from_db(GtkListStore *list_store, sqlite3 *db);

static GtkListStore *Teachers_ListStore_new(void) {
	return gtk_list_store_new(N_COLUMNS,
	                          G_TYPE_INT,
	                          G_TYPE_STRING);
}

void init_notebook_teachers(GtkBuilder *builder, sqlite3 *db) {
	GtkTreeView *teachers_tree_view = GTK_TREE_VIEW(
		gtk_builder_get_object(builder, "teachers_tree_view")
	);
	GtkListStore *list_store = Teachers_ListStore_new();
	set_Teachers_from_db(list_store, db);
	gtk_tree_view_set_model(teachers_tree_view, GTK_TREE_MODEL(list_store));

	gtk_tree_view_append_column(
		teachers_tree_view,
		gtk_tree_view_column_new_with_attributes(
			"Name",
			gtk_cell_renderer_text_new(), "text", COLUMN_STRING_name, NULL)
	);
}

static void set_Teachers_from_db(GtkListStore *list_store, sqlite3 *db) {
	sqlite3_stmt *stmt;
	sqlite3_prepare(db, "SELECT * FROM teachers", -1, &stmt, NULL);
	GtkTreeIter iter;
	while(sqlite3_step(stmt) == SQLITE_ROW) {
		gtk_list_store_append(list_store, &iter);
		Teacher teacher = Teacher_from_stmt(stmt);
		gtk_list_store_set(list_store, &iter,
		                   COLUMN_INT_pk,       teacher.pk,
		                   COLUMN_STRING_name,  teacher.name,
		                   -1);
	}
	sqlite3_finalize(stmt);
}
