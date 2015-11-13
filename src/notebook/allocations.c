#include "../libs.h"
#include "../callback.h"

#include "allocations.h"
#include "../db_tables/db.h"


typedef enum {
	COLUMN_INT_pk,
	COLUMN_INT_batch_pk,
	COLUMN_STRING_batch_name,
	COLUMN_INT_subject_pk,
	COLUMN_STRING_subject_name,
	COLUMN_INT_teacher_pk,
	COLUMN_STRING_teacher_name,
	N_COLUMNS
} TreeView_Allocations_E;

static void set_Allocations_from_db(GtkListStore *list_store, sqlite3 *db);


static void add_allocation_CB(GtkButton* button, CallBackData *data) {
	(void)button;
	GtkComboBox *subject_combo_box = (GtkComboBox *)gtk_builder_get_object(
		data->builder, "allocations_add_window_subject_combobox");
	GtkComboBox *batch_combo_box   = (GtkComboBox *)gtk_builder_get_object(
		data->builder, "allocations_add_window_batch_combobox");
	GtkComboBox *teacher_combo_box = (GtkComboBox *)gtk_builder_get_object(
		data->builder, "allocations_add_window_teacher_combobox");

	GtkTreeModel *subject_list_store =
		gtk_combo_box_get_model(subject_combo_box);
	GtkTreeModel *batch_list_store   =
		gtk_combo_box_get_model(batch_combo_box);
	GtkTreeModel *teacher_list_store =
		gtk_combo_box_get_model(teacher_combo_box);

	GtkTreeIter iter;
	Allocation allocation;

	if (gtk_combo_box_get_active_iter(subject_combo_box, &iter)) {
		gtk_tree_model_get(subject_list_store, &iter,
		                   0, &(allocation.subject), -1);
	}
	else {
		return;
	}
	if (gtk_combo_box_get_active_iter(teacher_combo_box, &iter)) {
		gtk_tree_model_get(teacher_list_store, &iter,
		                   0, &(allocation.teacher), -1);
	}
	else {
		return;
	}
	if (gtk_combo_box_get_active_iter(batch_combo_box, &iter)) {
		gtk_tree_model_get(batch_list_store, &iter,
		                   0, &(allocation.batch), -1);
	}
	else {
		return;
	}

	/* Because the number of allocations change */
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

	insert_Allocation(data->db, &allocation);
	GObject *window = gtk_builder_get_object(
		data->builder, "allocations_add_window");
	gtk_widget_hide((GtkWidget *)window);

	gtk_combo_box_set_active(subject_combo_box, -1);
	gtk_combo_box_set_active(batch_combo_box, -1);
	gtk_combo_box_set_active(teacher_combo_box, -1);

	refresh_notebook_allocations(data);
}


static void cancel_add_allocation_CB(GtkButton* button, CallBackData *data) {
	(void)button;
	GObject *window = gtk_builder_get_object(
		data->builder, "allocations_add_window");
	gtk_widget_hide((GtkWidget *)window);

	GtkComboBox *subject_combo_box = (GtkComboBox *)gtk_builder_get_object(
		data->builder, "allocations_add_window_subject_combobox");
	GtkComboBox *batch_combo_box   = (GtkComboBox *)gtk_builder_get_object(
		data->builder, "allocations_add_window_batch_combobox");
	GtkComboBox *teacher_combo_box = (GtkComboBox *)gtk_builder_get_object(
		data->builder, "allocations_add_window_teacher_combobox");

	gtk_combo_box_set_active(subject_combo_box, -1);
	gtk_combo_box_set_active(batch_combo_box, -1);
	gtk_combo_box_set_active(teacher_combo_box, -1);
}


static gboolean close_add_allocation_window_CB(GtkWidget *widget,
                                               GdkEvent  *event,
                                               CallBackData *data) {
	(void)widget;
	(void)event;
	cancel_add_allocation_CB(NULL, data);
	return TRUE;
}


void init_notebook_allocations(CallBackData *data) {
	GtkTreeView *allocations_tree_view = GTK_TREE_VIEW(
		gtk_builder_get_object(data->builder, "allocations_tree_view"));

	gtk_tree_view_append_column(
		allocations_tree_view,
		gtk_tree_view_column_new_with_attributes(
			"Batch",
			gtk_cell_renderer_text_new(),
			"text", COLUMN_STRING_batch_name,
			NULL));
	gtk_tree_view_append_column(
		allocations_tree_view,
		gtk_tree_view_column_new_with_attributes(
			"Subject",
			gtk_cell_renderer_text_new(),
			"text", COLUMN_STRING_subject_name,
			NULL));
	gtk_tree_view_append_column(
		allocations_tree_view,
		gtk_tree_view_column_new_with_attributes(
			"Teacher",
			gtk_cell_renderer_text_new(),
			"text", COLUMN_STRING_teacher_name,
			NULL));

	GObject *add_allocation_button = gtk_builder_get_object(
		data->builder, "allocations_add_window_ok_button");
	g_signal_connect(add_allocation_button, "clicked",
	                 G_CALLBACK(add_allocation_CB), data);
	GObject *cancel_add_allocation_button = gtk_builder_get_object(
		data->builder, "allocations_add_window_cancel_button");
	g_signal_connect(cancel_add_allocation_button, "clicked",
	                 G_CALLBACK(cancel_add_allocation_CB), data);
	GObject *add_allocation_window = gtk_builder_get_object(
		data->builder, "allocations_add_window");
	g_signal_connect(add_allocation_window, "delete-event",
	                 G_CALLBACK(close_add_allocation_window_CB), data);


	GtkComboBox *subject_combo_box = (GtkComboBox *)gtk_builder_get_object(
		data->builder, "allocations_add_window_subject_combobox");
	GtkComboBox *batch_combo_box   = (GtkComboBox *)gtk_builder_get_object(
		data->builder, "allocations_add_window_batch_combobox");
	GtkComboBox *teacher_combo_box = (GtkComboBox *)gtk_builder_get_object(
		data->builder, "allocations_add_window_teacher_combobox");


	GtkCellRenderer *combobox_text;
	combobox_text = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(subject_combo_box),
	                           combobox_text, TRUE);
	gtk_cell_layout_add_attribute(GTK_CELL_LAYOUT(subject_combo_box),
	                              combobox_text,
	                              "text", 1);
	combobox_text = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(teacher_combo_box),
	                           combobox_text, TRUE);
	gtk_cell_layout_add_attribute(GTK_CELL_LAYOUT(teacher_combo_box),
	                              combobox_text,
	                              "text", 1);
	combobox_text = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(batch_combo_box),
	                           combobox_text, TRUE);
	gtk_cell_layout_add_attribute(GTK_CELL_LAYOUT(batch_combo_box),
	                              combobox_text,
	                              "text", 1);
}


static void set_Allocations_from_db(GtkListStore *list_store, sqlite3 *db) {
	sqlite3_stmt *stmt;

	const gchar *sql = "SELECT "
		"allocations.pk,      "
		"allocations.batch,   "
		"allocations.subject, "
		"allocations.teacher, "
		"batches.name,        "
		"subjects.name,       "
		"teachers.name        "
			"FROM allocations "
				"INNER JOIN batches  ON allocations.batch=batches.pk    "
				"INNER JOIN teachers ON allocations.teacher=teachers.pk "
				"INNER JOIN subjects ON subjects.pk=allocations.subject;";

	sqlite3_prepare(db, sql, -1, &stmt, NULL);
	GtkTreeIter iter;
	while(sqlite3_step(stmt) == SQLITE_ROW) {
		gtk_list_store_append(list_store, &iter);
		Allocation allocation = Allocation_from_stmt(stmt);
		gtk_list_store_set(
			list_store, &iter,
			COLUMN_INT_pk,              allocation.pk,
			COLUMN_INT_batch_pk,        allocation.batch,
			COLUMN_INT_subject_pk,      allocation.subject,
			COLUMN_INT_teacher_pk,      allocation.teacher,
			COLUMN_STRING_batch_name,   sqlite3_column_text(stmt, 4),
			COLUMN_STRING_subject_name, sqlite3_column_text(stmt, 5),
			COLUMN_STRING_teacher_name, sqlite3_column_text(stmt, 6),
			-1);
	}
	sqlite3_finalize(stmt);
}


void refresh_notebook_allocations(CallBackData *data) {
	GtkTreeView *allocations_tree_view = GTK_TREE_VIEW(
		gtk_builder_get_object(data->builder, "allocations_tree_view"));
	GtkListStore *list_store =
		(GtkListStore *)gtk_tree_view_get_model(allocations_tree_view);
	gtk_list_store_clear(list_store);
	set_Allocations_from_db(list_store, data->db);
}
