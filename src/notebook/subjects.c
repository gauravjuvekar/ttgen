#include <gtk/gtk.h>

#include "subjects.h"
#include "../sqlite/sqlite3.h"
#include "../db_tables/subjects.h"

typedef enum {
	COLUMN_INT_pk,
	COLUMN_STRING_name,
	COLUMN_INT_frequency,
	COLUMN_INT_contiguous_time,
	COLUMN_INT_parallel,
	N_COLUMNS
} TreeView_Subjects_E;

static GtkListStore *Subjects_ListStore_new(void);
static void set_Subjects_from_db(GtkListStore *list_store, sqlite3 *db);

static GtkListStore *Subjects_ListStore_new(void) {
	return gtk_list_store_new(N_COLUMNS,
	                          G_TYPE_INT,
	                          G_TYPE_STRING,
	                          G_TYPE_INT,
	                          G_TYPE_INT,
	                          G_TYPE_INT);
}

void init_notebook_subjects(GtkBuilder *builder, sqlite3 *db) {
	GtkTreeView *subjects_tree_view = GTK_TREE_VIEW(
		gtk_builder_get_object(builder, "subjects_tree_view")
	);
	GtkListStore *list_store = Subjects_ListStore_new();
	set_Subjects_from_db(list_store, db);
	gtk_tree_view_set_model(subjects_tree_view, GTK_TREE_MODEL(list_store));

	gtk_tree_view_append_column(
		subjects_tree_view,
		gtk_tree_view_column_new_with_attributes(
			"Name",
			gtk_cell_renderer_text_new(), "text", COLUMN_STRING_name, NULL)
	);
	gtk_tree_view_append_column(
		subjects_tree_view,
		gtk_tree_view_column_new_with_attributes(
			"Frequency",
			gtk_cell_renderer_text_new(), "text", COLUMN_INT_frequency, NULL)
	);
	gtk_tree_view_append_column(
		subjects_tree_view,
		gtk_tree_view_column_new_with_attributes(
			"Contiguous time",
			gtk_cell_renderer_text_new(), "text", COLUMN_INT_contiguous_time,
			NULL)
	);
	gtk_tree_view_append_column(
		subjects_tree_view,
		gtk_tree_view_column_new_with_attributes(
			"Parallel",
			gtk_cell_renderer_text_new(), "text", COLUMN_INT_parallel, NULL)
	);

}

static void set_Subjects_from_db(GtkListStore *list_store, sqlite3 *db) {
	sqlite3_stmt *stmt;
	sqlite3_prepare(db, "SELECT * FROM subjects", -1, &stmt, NULL);
	GtkTreeIter iter;
	while(sqlite3_step(stmt) == SQLITE_ROW) {
		gtk_list_store_append(list_store, &iter);
		Subject subject = Subject_from_stmt(stmt);
		gtk_list_store_set(list_store, &iter,
		                   COLUMN_INT_pk,              subject.pk,
		                   COLUMN_STRING_name,         subject.name,
		                   COLUMN_INT_frequency,       subject.frequency,
		                   COLUMN_INT_contiguous_time, subject.contiguous_time,
		                   COLUMN_INT_parallel,        subject.parallel,
		                   -1);
	}
	sqlite3_finalize(stmt);
}
