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

#include "../core/genetic.h"
#include "table.h"


typedef enum {
	FILTER_SELECTION_ALL = 0,
	FILTER_SELECTION_TEACHER,
	FILTER_SELECTION_ROOM,
	FILTER_SELECTION_BATCH
} FilterSelection_E;


static void change_type_selection_combobox_CB(GtkComboBoxText *type_combobox,
                                              CallBackData *data) {
	const gchar *id = gtk_combo_box_get_active_id((GtkComboBox *)type_combobox);
	g_assert(id != NULL);
	g_assert(g_ascii_isdigit(id[0]));
	FilterSelection_E filter_type = (FilterSelection_E) id[0] - '0';

	GtkComboBox *filter_combobox = (GtkComboBox *)gtk_builder_get_object(
			data->builder, "table_view_filter_item_selection_combobox");

	gchar *model_name = NULL;
	switch(filter_type) {
		case FILTER_SELECTION_ALL:
			model_name = NULL;
			break;
		case FILTER_SELECTION_TEACHER:
			model_name = "teachers_list_store";
			break;
		case FILTER_SELECTION_ROOM:
			model_name = "rooms_list_store";
			break;
		case FILTER_SELECTION_BATCH:
			model_name = "batches_list_store";
			break;
		default:
			g_assert(FALSE);
	}

	if (model_name != NULL) {
		GtkTreeModel *model =
			(GtkTreeModel *)gtk_builder_get_object(data->builder, model_name);
		gtk_combo_box_set_model(filter_combobox, GTK_TREE_MODEL(model));
		gtk_widget_set_sensitive(GTK_WIDGET(filter_combobox), TRUE);
	}
	else {
		gtk_widget_set_sensitive(GTK_WIDGET(filter_combobox), FALSE);
		gtk_combo_box_set_model(filter_combobox, NULL);
	}

	/* To refresh the table view */
	g_signal_emit_by_name(
		gtk_builder_get_object(data->builder, "schedules_tree_selection"),
		"changed");
}


static void set_grid_batch(GtkGrid *grid,
                           gint schedule_pk,
                           gint batch_pk,
                           CallBackData *data) {
	sqlite3_stmt *stmt;
	gint sql_ret;

	gint row;
	for (row = 0; row < data->meta->n_time_slots_per_day; row++) {
		const gchar *name = g_strdup_printf("%d", row);
		GtkLabel *label = (GtkLabel *)gtk_label_new(name);
		gtk_grid_attach(grid, (GtkWidget *)label, 0, row + 1, 1, 1);
		g_free((gpointer)name);
	}

	gint column;
	for (column = 0;
	     column < (data->meta->n_time_slots / data->meta->n_time_slots_per_day);
	     column++) {
		const gchar *name = g_strdup_printf("Day %d", column);
		GtkLabel *label = (GtkLabel *)gtk_label_new(name);
		gtk_grid_attach(grid, (GtkWidget *)label, column + 1, 0, 1, 1);
		g_free((gpointer)name);
	}

	sql_ret = sqlite3_prepare(
		data->db,
		"SELECT "
		"schedule_allocs.slot, subjects.name, teachers.name, rooms.name "
		"FROM schedule_allocs "
			"JOIN allocations ON schedule_allocs.allocation=allocations.pk "
			"JOIN teachers    ON allocations.teacher=teachers.pk "
			"JOIN subjects    ON allocations.subject=subjects.pk "
			"JOIN rooms       ON (schedule_allocs.slot % "
			                     "(SELECT COUNT() FROM rooms))=rooms.pk "
				"WHERE schedule_allocs.schedule=:schedule_pk AND "
				      "allocations.batch=:batch_pk;",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":schedule_pk"), schedule_pk);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":batch_pk"), batch_pk);
	g_assert(sql_ret == SQLITE_OK);
	while ((sql_ret = sqlite3_step(stmt)) == SQLITE_ROW) {
		gint slot = sqlite3_column_int(stmt, 0);
		const gchar *subject = (const gchar*)sqlite3_column_text(stmt, 1);
		const gchar *teacher = (const gchar*)sqlite3_column_text(stmt, 2);
		const gchar *room    = (const gchar*)sqlite3_column_text(stmt, 3);

		const gchar *label_text = g_strdup_printf("R:%s\nS:%s\nT:%s\n",
												  room, subject, teacher);

		GtkLabel *label = (GtkLabel *)gtk_label_new(label_text);

		gtk_grid_attach(grid, (GtkWidget *)label,
						(time_slot_from_slot(slot, data->meta) /
						 data->meta->n_time_slots_per_day) + 1 ,
						(time_slot_from_slot(slot, data->meta) %
						 data->meta->n_time_slots_per_day) + 1 ,
						1, 1);
		g_free((gpointer)label_text);
	}
}


static void set_grid_teacher(GtkGrid *grid,
                             gint schedule_pk,
                             gint teacher_pk,
                             CallBackData *data) {
	sqlite3_stmt *stmt;
	gint sql_ret;

	gint row;
	for (row = 0; row < data->meta->n_time_slots_per_day; row++) {
		const gchar *name = g_strdup_printf("%d", row);
		GtkLabel *label = (GtkLabel *)gtk_label_new(name);
		gtk_grid_attach(grid, (GtkWidget *)label, 0, row + 1, 1, 1);
		g_free((gpointer)name);
	}

	gint column;
	for (column = 0;
	     column < (data->meta->n_time_slots / data->meta->n_time_slots_per_day);
	     column++) {
		const gchar *name = g_strdup_printf("Day %d", column);
		GtkLabel *label = (GtkLabel *)gtk_label_new(name);
		gtk_grid_attach(grid, (GtkWidget *)label, column + 1, 0, 1, 1);
		g_free((gpointer)name);
	}

	sql_ret = sqlite3_prepare(
		data->db,
		"SELECT "
		"schedule_allocs.slot, subjects.name, batches.name, rooms.name "
		"FROM schedule_allocs "
			"JOIN allocations ON schedule_allocs.allocation=allocations.pk "
			"JOIN batches     ON allocations.batch=batches.pk "
			"JOIN subjects    ON allocations.subject=subjects.pk "
			"JOIN rooms       ON (schedule_allocs.slot % "
			                     "(SELECT COUNT() FROM rooms))=rooms.pk "
				"WHERE schedule_allocs.schedule=:schedule_pk AND "
				      "allocations.teacher=:teacher_pk;",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":schedule_pk"), schedule_pk);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":teacher_pk"), teacher_pk);
	g_assert(sql_ret == SQLITE_OK);
	while ((sql_ret = sqlite3_step(stmt)) == SQLITE_ROW) {
		gint slot = sqlite3_column_int(stmt, 0);
		const gchar *subject = (const gchar*)sqlite3_column_text(stmt, 1);
		const gchar *batch   = (const gchar*)sqlite3_column_text(stmt, 2);
		const gchar *room    = (const gchar*)sqlite3_column_text(stmt, 3);

		const gchar *label_text = g_strdup_printf("R:%s\nS:%s\nB:%s\n",
												  room, subject, batch);

		GtkLabel *label = (GtkLabel *)gtk_label_new(label_text);

		gtk_grid_attach(grid, (GtkWidget *)label,
						(time_slot_from_slot(slot, data->meta) /
						 data->meta->n_time_slots_per_day) + 1 ,
						(time_slot_from_slot(slot, data->meta) %
						 data->meta->n_time_slots_per_day) + 1 ,
						1, 1);
		g_free((gpointer)label_text);
	}
}


static void set_grid_room(GtkGrid *grid,
                          gint schedule_pk,
                          gint room_pk,
                          CallBackData *data) {
	sqlite3_stmt *stmt;
	gint sql_ret;

	gint row;
	for (row = 0; row < data->meta->n_time_slots_per_day; row++) {
		const gchar *name = g_strdup_printf("%d", row);
		GtkLabel *label = (GtkLabel *)gtk_label_new(name);
		gtk_grid_attach(grid, (GtkWidget *)label, 0, row + 1, 1, 1);
		g_free((gpointer)name);
	}

	gint column;
	for (column = 0;
	     column < (data->meta->n_time_slots / data->meta->n_time_slots_per_day);
	     column++) {
		const gchar *name = g_strdup_printf("Day %d", column);
		GtkLabel *label = (GtkLabel *)gtk_label_new(name);
		gtk_grid_attach(grid, (GtkWidget *)label, column + 1, 0, 1, 1);
		g_free((gpointer)name);
	}

	sql_ret = sqlite3_prepare(
		data->db,
		"SELECT "
		"schedule_allocs.slot, subjects.name, batches.name, teachers.name "
		"FROM schedule_allocs "
			"JOIN allocations ON schedule_allocs.allocation=allocations.pk "
			"JOIN batches     ON allocations.batch=batches.pk "
			"JOIN subjects    ON allocations.subject=subjects.pk "
			"JOIN teachers    ON allocations.teacher=teachers.pk "
				"WHERE schedule_allocs.schedule=:schedule_pk AND "
				      "(schedule_allocs.slot % "
				       "(SELECT COUNT() FROM rooms))=:room_pk;",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":schedule_pk"), schedule_pk);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":room_pk"), room_pk);
	g_assert(sql_ret == SQLITE_OK);
	while ((sql_ret = sqlite3_step(stmt)) == SQLITE_ROW) {
		gint slot = sqlite3_column_int(stmt, 0);
		const gchar *subject = (const gchar*)sqlite3_column_text(stmt, 1);
		const gchar *batch   = (const gchar*)sqlite3_column_text(stmt, 2);
		const gchar *teacher = (const gchar*)sqlite3_column_text(stmt, 3);

		const gchar *label_text = g_strdup_printf("T:%s\nS:%s\nB:%s\n",
												  teacher, subject, batch);

		GtkLabel *label = (GtkLabel *)gtk_label_new(label_text);

		gtk_grid_attach(grid, (GtkWidget *)label,
						(time_slot_from_slot(slot, data->meta) /
						 data->meta->n_time_slots_per_day) + 1 ,
						(time_slot_from_slot(slot, data->meta) %
						 data->meta->n_time_slots_per_day) + 1 ,
						1, 1);
		g_free((gpointer)label_text);
	}
}


static void set_grid_all(GtkGrid *grid, gint schedule_pk, CallBackData *data) {
	sqlite3_stmt *stmt;
	gint sql_ret;
	sql_ret = sqlite3_prepare(data->db, "SELECT name FROM rooms;",
							  -1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	gint row;
	for (row = 0; (sql_ret = sqlite3_step(stmt)) == SQLITE_ROW; row++) {
		const gchar *name = (const gchar *)sqlite3_column_text(stmt, 0);
		GtkLabel *label = (GtkLabel *)gtk_label_new(name);
		gtk_grid_attach(grid, (GtkWidget *)label, 0, row + 1, 1, 1);
	}
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	gint column;
	for (column = 0; column < data->meta->n_time_slots; column++) {
		const gchar *name = g_strdup_printf("%d", column);
		GtkLabel *label = (GtkLabel *)gtk_label_new(name);
		gtk_grid_attach(grid, (GtkWidget *)label, column + 1, 0, 1, 1);
		g_free((gpointer)name);
	}

	sql_ret = sqlite3_prepare(
		data->db,
		"SELECT "
		"schedule_allocs.slot, teachers.name, subjects.name, batches.name "
		"FROM schedule_allocs "
			"JOIN allocations ON schedule_allocs.allocation=allocations.pk "
			"JOIN batches     ON allocations.batch=batches.pk "
			"JOIN subjects    ON allocations.subject=subjects.pk "
			"JOIN teachers    ON allocations.teacher=teachers.pk "
				"WHERE schedule_allocs.schedule=:pk;",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":pk"), schedule_pk);
	g_assert(sql_ret == SQLITE_OK);
	while ((sql_ret = sqlite3_step(stmt)) == SQLITE_ROW) {
		gint slot = sqlite3_column_int(stmt, 0);
		const gchar *teacher = (const gchar*)sqlite3_column_text(stmt, 1);
		const gchar *subject = (const gchar*)sqlite3_column_text(stmt, 2);
		const gchar *batch   = (const gchar*)sqlite3_column_text(stmt, 3);

		const gchar *label_text = g_strdup_printf("T:%s\nS:%s\nB:%s\n",
												  teacher, subject, batch);

		GtkLabel *label = (GtkLabel *)gtk_label_new(label_text);

		gtk_grid_attach(grid, (GtkWidget *)label,
						time_slot_from_slot(slot, data->meta) + 1 ,
						room_from_slot(slot, data->meta) + 1,
						1, 1);
		g_free((gpointer)label_text);
	}
}


static void refresh_table_CB(GtkWidget* widget, CallBackData *data) {
	(void)widget;
	GtkTreeSelection *selection =
		(GtkTreeSelection *)gtk_builder_get_object(data->builder,
		                                           "schedules_tree_selection");
	GtkTreeModel *model;
	GtkTreeIter  iter;

	if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
		gint pk;
		gtk_tree_model_get(model, &iter, 0, &pk, -1);

		GtkViewport *viewport =
			(GtkViewport *)gtk_builder_get_object(data->builder,
			                                      "timetable_viewport");
		GtkGrid *grid = (GtkGrid *)gtk_bin_get_child((GtkBin *)viewport);
		if (grid) {
			gtk_widget_destroy((GtkWidget *)grid);
		}

		grid = (GtkGrid *)gtk_grid_new();
		gtk_container_add((GtkContainer *)viewport, (GtkWidget *)grid);
		gtk_grid_set_row_spacing(grid, 6);
		gtk_grid_set_column_spacing(grid, 6);
		gtk_grid_set_row_homogeneous(grid, TRUE);
		gtk_grid_set_column_homogeneous(grid, TRUE);

		GtkComboBoxText *type_combobox =
			(GtkComboBoxText *)gtk_builder_get_object(
				data->builder, "table_view_type_selection_combobox");

		const gchar *filter_id = gtk_combo_box_get_active_id(
			(GtkComboBox *)type_combobox);
		g_assert(filter_id != NULL);
		g_assert(g_ascii_isdigit(filter_id[0]));
		FilterSelection_E filter_type = (FilterSelection_E) filter_id[0] - '0';

		GtkComboBox *filter_combobox = (GtkComboBox *)gtk_builder_get_object(
				data->builder, "table_view_filter_item_selection_combobox");

		GtkTreeModel *filter_model = gtk_combo_box_get_model(filter_combobox);
		GtkTreeIter iter;
		gboolean iter_set = gtk_combo_box_get_active_iter(filter_combobox,
														  &iter);
		gint filter_pk;
		if (iter_set) {
			gtk_tree_model_get(filter_model, &iter, 0, &filter_pk, -1);
		}
		switch(filter_type) {
			case FILTER_SELECTION_ALL:
				set_grid_all(grid, pk, data);
				break;
			case FILTER_SELECTION_TEACHER:
				if (iter_set) {
					set_grid_teacher(grid, pk, filter_pk, data);
				}
				break;
			case FILTER_SELECTION_ROOM:
				if (iter_set) {
					set_grid_room(grid, pk, filter_pk, data);
				}
				break;
			case FILTER_SELECTION_BATCH:
				if (iter_set) {
					set_grid_batch(grid, pk, filter_pk, data);
				}
				break;
			default:
				g_assert(FALSE);
		}

		gtk_widget_show_all((GtkWidget *)grid);
	}
}

static void table_view_filter_selection_CB(GtkComboBoxText *filter_combobox,
                                           CallBackData *data) {
	(void)filter_combobox;
	g_signal_emit_by_name(
		gtk_builder_get_object(data->builder, "schedules_tree_selection"),
		"changed");
}

void init_table_view(CallBackData *data) {
	g_signal_connect(gtk_builder_get_object(data->builder,
	                                        "schedules_tree_selection"),
	                 "changed", G_CALLBACK(refresh_table_CB), data);

	g_signal_connect(
		gtk_builder_get_object(data->builder,
		                       "table_view_type_selection_combobox"),
		"changed", G_CALLBACK(change_type_selection_combobox_CB), data);
	g_signal_connect(
		gtk_builder_get_object(data->builder,
		                       "table_view_filter_item_selection_combobox"),
		"changed", G_CALLBACK(table_view_filter_selection_CB), data);


	GtkComboBox *filter_combobox = (GtkComboBox *)gtk_builder_get_object(
			data->builder, "table_view_filter_item_selection_combobox");
	GtkCellRenderer *combobox_text = gtk_cell_renderer_combo_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(filter_combobox),
							   combobox_text, TRUE);
	gtk_cell_layout_add_attribute(GTK_CELL_LAYOUT(filter_combobox),
								  combobox_text,
								  "text", 1);

}
