#include "../libs.h"
#include "../callback.h"

#include "subjects.h"
#include "../db_tables/subjects.h"


typedef enum {
	COLUMN_INT_pk,
	COLUMN_STRING_name,
	N_COLUMNS
} TreeView_Subjects_E;

static void set_Subjects_from_db(GtkListStore *list_store, sqlite3 *db);


static void add_subject_CB(GtkButton* button, CallBackData *data) {
	(void)button;
	GObject *window = gtk_builder_get_object(
		data->builder, "subjects_add_window");
	gtk_widget_show_all((GtkWidget *)window);

	GObject *name_entry = gtk_builder_get_object(
		data->builder, "subjects_add_window_name_entry");
	Subject subject = (Subject) {
		.name = gtk_entry_get_text((GtkEntry *)name_entry)
	};

	insert_Subject(data->db, &subject);
	gtk_widget_hide((GtkWidget *)window);
	gtk_entry_set_text((GtkEntry *)name_entry, "");

	refresh_notebook_subjects(data);
}


static void cancel_add_subject_CB(GtkButton *button, CallBackData *data) {
	(void)button;
	GObject *window = gtk_builder_get_object(
		data->builder, "subjects_add_window");
	GObject *name_entry = gtk_builder_get_object(
		data->builder, "subjects_add_window_name_entry");
	gtk_widget_hide((GtkWidget *)window);
	gtk_entry_set_text((GtkEntry *)name_entry, "");
}


static gboolean close_add_subject_window_CB(GtkWidget *widget,
                                            GdkEvent  *event,
                                            CallBackData *data) {
	(void)widget;
	(void)event;
	cancel_add_subject_CB(NULL, data);
	return TRUE;
}


void init_notebook_subjects(CallBackData *data) {
	GtkTreeView *subjects_tree_view = GTK_TREE_VIEW(
		gtk_builder_get_object(data->builder, "subjects_tree_view"));

	gtk_tree_view_append_column(
		subjects_tree_view,
		gtk_tree_view_column_new_with_attributes(
			"Name",
			gtk_cell_renderer_text_new(),
			"text", COLUMN_STRING_name,
			NULL));


	GObject *add_subject_button = gtk_builder_get_object(
		data->builder, "subjects_add_window_ok_button");
	g_signal_connect(add_subject_button, "clicked",
	                 G_CALLBACK(add_subject_CB), data);
	GObject *cancel_add_subject_button = gtk_builder_get_object(
		data->builder, "subjects_add_window_cancel_button");
	g_signal_connect(cancel_add_subject_button, "clicked",
	                 G_CALLBACK(cancel_add_subject_CB), data);
	GObject *add_subject_window = gtk_builder_get_object(data->builder,
													   "subjects_add_window");
	g_signal_connect(add_subject_window, "delete-event",
	                 G_CALLBACK(close_add_subject_window_CB), data);
}


static void set_Subjects_from_db(GtkListStore *list_store, sqlite3 *db) {
	sqlite3_stmt *stmt;
	sqlite3_prepare(db, "SELECT pk, name FROM subjects;", -1, &stmt, NULL);
	GtkTreeIter iter;
	while(sqlite3_step(stmt) == SQLITE_ROW) {
		gtk_list_store_append(list_store, &iter);
		Subject subject = Subject_from_stmt(stmt);
		gtk_list_store_set(list_store, &iter,
		                   COLUMN_INT_pk,      subject.pk,
		                   COLUMN_STRING_name, subject.name,
		                   -1);
	}
	sqlite3_finalize(stmt);
}


void refresh_notebook_subjects(CallBackData *data) {
	GtkTreeView *subjects_tree_view = GTK_TREE_VIEW(
		gtk_builder_get_object(data->builder, "subjects_tree_view"));
	GtkListStore *list_store =
		(GtkListStore *)gtk_tree_view_get_model(subjects_tree_view);
	gtk_list_store_clear(list_store);
	set_Subjects_from_db(list_store, data->db);
}
