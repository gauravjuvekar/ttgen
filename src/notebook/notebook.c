#include <gtk/gtk.h>
#include <glib.h>
#include "../sqlite/sqlite3.h"
#include "notebook.h"

void init_notebooks(CallBackData *cb_data) {
	init_notebook_rooms(cb_data->builder, cb_data->db);
	init_notebook_subjects(cb_data->builder, cb_data->db);
	init_notebook_teachers(cb_data->builder, cb_data->db);
	init_notebook_batches(cb_data->builder, cb_data->db);
	g_signal_connect(gtk_builder_get_object(cb_data->builder,
	                                        "db_entry_insert_new_button"),
	                 "clicked", G_CALLBACK(add_button_CB), cb_data);
}

void add_button_CB(GtkButton *button, CallBackData *data) {
	(void)button;
	GObject *notebook = gtk_builder_get_object(data->builder, "db_notebook");
	NotebookTabs tab = gtk_notebook_get_current_page((GtkNotebook *)notebook);

	gchar *builder_window_name = NULL;
	switch(tab) {
		case TAB_ALLOCATION:
			break;
			builder_window_name = "allocations_add_window";
		case TAB_TEACHERS:
			builder_window_name = "teachers_add_window";
			break;
		case TAB_ROOMS:
			builder_window_name = "rooms_add_window";
			break;
		case TAB_BATCHES:
			builder_window_name = "batches_add_window";
			break;
		case TAB_SUBJECTS:
			builder_window_name = "subjects_add_window";
			break;
		default:
			g_assert(FALSE);
	}

	GObject *window = gtk_builder_get_object(data->builder,
	                                         builder_window_name);
	gtk_widget_show_all(GTK_WIDGET(window));
}

void remove_button_CB(GtkButton *button, CallBackData *data) {
	(void)button;
	GObject *notebook = gtk_builder_get_object(data->builder, "db_notebook");
	NotebookTabs tab = gtk_notebook_get_current_page((GtkNotebook *)notebook);

	GtkTreeSelection *selection = NULL;

	switch(tab) {
		case TAB_ALLOCATION:
			break;
		case TAB_TEACHERS:
			break;
		case TAB_ROOMS:
			break;
		case TAB_BATCHES:
			break;
		case TAB_SUBJECTS:
			break;
		default:
			g_assert(FALSE);
	}

}
