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

#include "schedules.h"

typedef enum {
	COLUMN_INT_pk,
	COLUMN_FLOAT_fitness,
	N_COLUMNS
} TreeView_Schedules_E;

static void set_Schedules_from_db(GtkListStore *list_store, sqlite3 *db);


void init_notebook_schedules(CallBackData *data) {
	GtkTreeView *schedules_tree_view = GTK_TREE_VIEW(
		gtk_builder_get_object(data->builder, "schedules_tree_view"));

	gtk_tree_view_append_column(
		schedules_tree_view,
		gtk_tree_view_column_new_with_attributes(
			"Fitness",
			gtk_cell_renderer_text_new(),
			"text", COLUMN_FLOAT_fitness,
			NULL));
}


static void set_Schedules_from_db(GtkListStore *list_store, sqlite3 *db) {
	sqlite3_stmt *stmt;
	sqlite3_prepare(db, "SELECT pk, fitness FROM schedules;",
	                -1, &stmt, NULL);
	GtkTreeIter iter;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		gtk_list_store_append(list_store, &iter);

		gint pk = sqlite3_column_int(stmt, 0);
		gfloat fitness = sqlite3_column_double(stmt, 1);

		gtk_list_store_set(list_store, &iter,
		                   COLUMN_INT_pk,        pk,
		                   COLUMN_FLOAT_fitness, fitness,
		                   -1);
	}
	sqlite3_finalize(stmt);
}


void refresh_notebook_schedules(CallBackData *data) {
	GtkTreeView *schedules_tree_view = GTK_TREE_VIEW(
		gtk_builder_get_object(data->builder, "schedules_tree_view"));
	GtkListStore *list_store =
		(GtkListStore *)gtk_tree_view_get_model(schedules_tree_view);
	gtk_list_store_clear(list_store);
	set_Schedules_from_db(list_store, data->db);
}
