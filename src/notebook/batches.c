/*  TTgen: An application to generate timetables using a genetic algorithm.
 *  Copyright (C) 2015  Gaurav Juvekar
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "../libs.h"
#include "../callback.h"

#include "batches.h"
#include "../db_tables/batches.h"


typedef enum {
	COLUMN_INT_pk,
	COLUMN_STRING_name,
	COLUMN_INT_heads,
	COLUMN_INT_parent,
	N_COLUMNS
} TreeView_Batches_E;

static void set_Batches_from_db(GtkListStore *list_store, sqlite3 *db);


static void add_batch_CB(GtkButton* button, CallBackData *data) {
	(void)button;
	GObject *name_entry = gtk_builder_get_object(
		data->builder, "batches_add_window_name_entry");
	GObject *heads_spin_button = gtk_builder_get_object(
		data->builder, "batches_add_window_heads_spinbutton");
	GtkComboBox *combo_box = (GtkComboBox *)gtk_builder_get_object(
		data->builder, "batches_add_window_parent_combobox");
	GtkTreeModel *list_store = gtk_combo_box_get_model(combo_box);
	GtkTreeIter iter;
	gint parent_pk;
	if (gtk_combo_box_get_active_iter(combo_box, &iter)) {
		gtk_tree_model_get(list_store, &iter, 0, &parent_pk, -1);
	}
	else {
		parent_pk = 0;
	}
	Batch batch = (Batch) {
		.name = gtk_entry_get_text((GtkEntry *)name_entry),
		.heads = gtk_spin_button_get_value_as_int(
			 (GtkSpinButton *)heads_spin_button),
		.parent = parent_pk
	};

	insert_Batch(data->db, &batch);
	GObject *window = gtk_builder_get_object(
		data->builder, "batches_add_window");
	gtk_widget_hide((GtkWidget *)window);
	gtk_entry_set_text((GtkEntry *)name_entry, "");
	gtk_combo_box_set_active(combo_box, -1);

	refresh_notebook_batches(data);
}


static void cancel_add_batch_CB(GtkButton* button, CallBackData *data) {
	(void)button;
	GObject *name_entry = gtk_builder_get_object(
		data->builder, "batches_add_window_name_entry");
	GObject *window = gtk_builder_get_object(
		data->builder, "batches_add_window");
	gtk_widget_hide((GtkWidget *)window);
	gtk_entry_set_text((GtkEntry *)name_entry, "");
	GtkComboBox *combo_box = (GtkComboBox *)gtk_builder_get_object(
		data->builder, "batches_add_window_parent_combobox");
	gtk_combo_box_set_active(combo_box, -1);
}


static gboolean close_add_batch_window_CB(GtkWidget *widget,
                                          GdkEvent  *event,
                                          CallBackData *data) {
	(void)widget;
	(void)event;
	cancel_add_batch_CB(NULL, data);
	return TRUE;
}


void init_notebook_batches(CallBackData *data) {
	GtkTreeView *batches_tree_view = GTK_TREE_VIEW(
		gtk_builder_get_object(data->builder, "batches_tree_view"));

	gtk_tree_view_append_column(
		batches_tree_view,
		gtk_tree_view_column_new_with_attributes(
			"Name",
			gtk_cell_renderer_text_new(),
			"text", COLUMN_STRING_name,
			NULL));
	gtk_tree_view_append_column(
		batches_tree_view,
		gtk_tree_view_column_new_with_attributes(
			"Heads",
			gtk_cell_renderer_text_new(),
			"text", COLUMN_INT_heads,
			NULL));
	gtk_tree_view_append_column(
		batches_tree_view,
		gtk_tree_view_column_new_with_attributes(
			"Parent",
			gtk_cell_renderer_text_new(),
			"text", COLUMN_INT_parent,
			NULL));

	GObject *add_batch_button = gtk_builder_get_object(
		data->builder, "batches_add_window_ok_button");
	g_signal_connect(add_batch_button, "clicked",
	                 G_CALLBACK(add_batch_CB), data);
	GObject *cancel_add_batch_button = gtk_builder_get_object(
		data->builder, "batches_add_window_cancel_button");
	g_signal_connect(cancel_add_batch_button, "clicked",
	                 G_CALLBACK(cancel_add_batch_CB), data);
	GObject *add_batch_window = gtk_builder_get_object(data->builder,
													   "batches_add_window");
	g_signal_connect(add_batch_window, "delete-event",
	                 G_CALLBACK(close_add_batch_window_CB), data);

	GObject *batches_parent_combobox = gtk_builder_get_object(
		data->builder, "batches_add_window_parent_combobox");
	GtkCellRenderer *combobox_text = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(batches_parent_combobox),
	                           combobox_text, TRUE);
	gtk_cell_layout_add_attribute(GTK_CELL_LAYOUT(batches_parent_combobox),
	                              combobox_text,
	                              "text", COLUMN_STRING_name);
}


static void set_Batches_from_db(GtkListStore *list_store, sqlite3 *db) {
	sqlite3_stmt *stmt;
	sqlite3_prepare(db, "SELECT pk, name, heads, parent FROM batches;",
	                -1, &stmt, NULL);
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
		gtk_builder_get_object(data->builder, "batches_tree_view"));
	GtkListStore *list_store =
		(GtkListStore *)gtk_tree_view_get_model(batches_tree_view);
	gtk_list_store_clear(list_store);
	set_Batches_from_db(list_store, data->db);
}
