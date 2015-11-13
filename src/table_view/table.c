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

static void refresh_table_CB(GtkWidget* widget, CallBackData *data) {
	(void)widget;
	GtkTreeSelection *selection =
		(GtkTreeSelection *)gtk_builder_get_object(data->builder,
		                                           "schedules_tree_selection");
	GtkTreeModel *model;
	GtkTreeIter  iter;

	Meta meta = Meta_from_db(data->db);

	if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
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
		for (column = 0; column < meta.n_time_slots; column++) {
			const gchar *name = g_strdup_printf("%d", column);
			GtkLabel *label = (GtkLabel *)gtk_label_new(name);
			gtk_grid_attach(grid, (GtkWidget *)label, column + 1, 0, 1, 1);
			g_free((gpointer)name);
		}

		gint pk;
		gtk_tree_model_get(model, &iter, 0, &pk, -1);


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
			stmt, sqlite3_bind_parameter_index(stmt, ":pk"), pk);
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
							time_slot_from_slot(slot, &meta) + 1 ,
			                room_from_slot(slot, &meta) + 1,
			                1, 1);
			g_free((gpointer)label_text);
		}
		gtk_widget_show_all((GtkWidget *)grid);
	}
}


void init_table_view(CallBackData *data) {
	g_signal_connect(gtk_builder_get_object(data->builder,
	                                        "schedules_tree_selection"),
	                 "changed", G_CALLBACK(refresh_table_CB), data);
}
