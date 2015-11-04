#include "../main.h"

#include "notebook.h"
#include "../db_tables/db.h"

void init_notebooks(CallBackData *cb_data) {
	init_notebook_rooms(cb_data);
	init_notebook_subjects(cb_data);
	init_notebook_teachers(cb_data);
	init_notebook_batches(cb_data);
	g_signal_connect(gtk_builder_get_object(cb_data->builder,
	                                        "db_entry_insert_new_button"),
	                 "clicked", G_CALLBACK(add_button_CB), cb_data);
	g_signal_connect(gtk_builder_get_object(cb_data->builder,
	                                        "db_entry_remove_button"),
	                 "clicked", G_CALLBACK(remove_button_CB), cb_data);
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

	gchar *tree_selection_object = NULL;
	remove_DB_func remove_entry_func;

	switch(tab) {
		case TAB_ALLOCATION:
			break;
		case TAB_TEACHERS:
			tree_selection_object = "teachers_tree_view_selection";
			remove_entry_func = remove_Teacher;
			break;
		case TAB_ROOMS:
			tree_selection_object = "rooms_tree_view_selection";
			remove_entry_func = remove_Room;
			break;
		case TAB_BATCHES:
			tree_selection_object = "batches_tree_view_selection";
			remove_entry_func = remove_Batch;
			break;
		case TAB_SUBJECTS:
			tree_selection_object = "subjects_tree_view_selection";
			remove_entry_func  = remove_Subject;
			break;
		default:
			g_assert(FALSE);
	}
	GObject *selection = gtk_builder_get_object(data->builder,
	                                            tree_selection_object);
	GtkTreeModel *model;
	GtkTreeIter  iter;
	if (gtk_tree_selection_get_selected((GtkTreeSelection *)selection,
	                                    &model, &iter)) {
		gint pk;
		gtk_tree_model_get(model, &iter, 0, &pk, -1);
		remove_entry_func(data->db, pk);

		refresh_notebook_rooms(data);
		refresh_notebook_subjects(data);
		refresh_notebook_batches(data);
		refresh_notebook_teachers(data);
	}
}
