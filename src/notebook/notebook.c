#include "../libs.h"
#include "../callback.h"

#include "notebook.h"
#include "../db_tables/db.h"

static void switch_tab_sensitive_CB(GtkNotebook  *notebook,
                                    GtkWidget    *page,
									guint        page_num,
                                    CallBackData *data) {
	(void)page;

	GtkButtonBox *db_edit_buttons =
		(GtkButtonBox *)gtk_builder_get_object(
			data->builder, "db_edit_buttons");

	switch(page_num) {
		case TAB_ALLOCATIONS:
		case TAB_TEACHERS:
		case TAB_ROOMS:
		case TAB_BATCHES:
		case TAB_SUBJECTS:
			gtk_widget_set_sensitive((GtkWidget *)db_edit_buttons, TRUE);
			break;
		case TAB_SCHEDULES:
			gtk_widget_set_sensitive((GtkWidget *)db_edit_buttons, FALSE);
			break;
		default:
			g_assert(FALSE);
	}
}

void init_notebooks(CallBackData *cb_data) {
	init_notebook_rooms(cb_data);
	init_notebook_subjects(cb_data);
	init_notebook_teachers(cb_data);
	init_notebook_batches(cb_data);
	init_notebook_allocations(cb_data);
	init_notebook_schedules(cb_data);
	g_signal_connect(gtk_builder_get_object(cb_data->builder,
	                                        "db_entry_insert_new_button"),
	                 "clicked", G_CALLBACK(add_button_CB), cb_data);
	g_signal_connect(gtk_builder_get_object(cb_data->builder,
	                                        "db_entry_remove_button"),
	                 "clicked", G_CALLBACK(remove_button_CB), cb_data);
	g_signal_connect(gtk_builder_get_object(cb_data->builder,
	                                        "db_notebook"),
	                 "switch-page",
	                 G_CALLBACK(switch_tab_sensitive_CB), cb_data);
}


void refresh_notebooks(CallBackData *cb_data) {
	refresh_notebook_rooms(cb_data);
	refresh_notebook_subjects(cb_data);
	refresh_notebook_teachers(cb_data);
	refresh_notebook_batches(cb_data);
	refresh_notebook_allocations(cb_data);
	refresh_notebook_schedules(cb_data);
}


void add_button_CB(GtkButton *button, CallBackData *data) {
	(void)button;
	GObject *notebook = gtk_builder_get_object(data->builder, "db_notebook");
	NotebookTabs tab = gtk_notebook_get_current_page((GtkNotebook *)notebook);

	gchar *builder_window_name = NULL;
	switch(tab) {
		case TAB_ALLOCATIONS:
			builder_window_name = "allocations_add_window";
			break;
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
		case TAB_ALLOCATIONS:
			tree_selection_object = "allocations_tree_view_selection";
			remove_entry_func = remove_Allocation;
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

		/* Set db schedules to be invalid as allocations change */
		sqlite3_stmt *stmt;
		gint sql_ret;
		sql_ret = sqlite3_prepare(
			data->db,
			"INSERT OR REPLACE INTO meta(key, int_value) "
			"VALUES(\"db_schedules_valid\", :schedules_valid);",
			-1, &stmt, NULL);
		g_assert(sql_ret == SQLITE_OK);
		sql_ret = sqlite3_bind_int(
			stmt, sqlite3_bind_parameter_index(stmt, ":schedules_valid"),
			0);
		g_assert(sql_ret == SQLITE_OK);
		sql_ret = sqlite3_step(stmt);
		g_assert(sql_ret == SQLITE_DONE);
		sql_ret = sqlite3_finalize(stmt);
		g_assert(sql_ret == SQLITE_OK);

		refresh_notebook_rooms(data);
		refresh_notebook_subjects(data);
		refresh_notebook_batches(data);
		refresh_notebook_teachers(data);
		refresh_notebook_allocations(data);
	}
}
