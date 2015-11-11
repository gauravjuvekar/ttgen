#include "../main.h"

#include "table.h"
#include "../main.h"
#include "../core/genetic.h"

static void refresh_table_CB(GtkWidget* widget, CallBackData *data) {
	(void)widget;
	GtkTreeSelection *selection =
		(GtkTreeSelection *)gtk_builder_get_object(data->builder,
		                                           "schedules_tree_selection");
	GtkTreeModel *model;
	GtkTreeIter  iter;

	Meta meta = Meta_from_db(data->db);

	if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
		GtkViewport *viewport = gtk_builder_get_object(data->builder,
		                                               "timetable_viewport");
		GtkGrid *grid = (GtkGrid *)gtk_bin_get_child(viewport);
		gtk_widget_destroy((GtkWidget *)grid);

		grid = (GtkGrid *)gtk_grid_new();
		gtk_container_add((GtkContainer *)viewport, (GtkWidget *)grid);

		gint pk;
		gtk_tree_model_get(model, &iter, 0, &pk, -1);

		sqlite3_stmt *stmt;
		gint sql_ret;

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
							time_slot_from_slot(slot, &meta) ,
			                room_from_slot(slot, &meta),
			                1, 1);
			g_free((gpointer)label_text);
		}
	}
}


void init_table_view(CallBackData *data) {
	g_signal_connect(gtk_builder_get_object(data->builder,
	                                        "schedules_tree_selection"),
	                 "changed", G_CALLBACK(refresh_table_CB), data);
}
