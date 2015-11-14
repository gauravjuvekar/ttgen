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


#include "main.h"

#include "notebook/notebook.h"
#include "menubar/prefs.h"
#include "menubar/file.h"
#include "menubar/help.h"
#include "core/interface.h"
#include "table_view/table.h"
#include "db_tables/db.h"
#include "db_tables/meta.h"


const char gui_builder_string[] = {
#include "gui.ui.hex.inc"
	, '\0'
};


void quit_CB(GtkWidget *widget, CallBackData *data) {
	(void)widget;
	sqlite3_close(data->db);
	gtk_main_quit();
}


static void init_set_sensitive(CallBackData *data) {
	gtk_widget_set_sensitive(
		(GtkWidget *)gtk_builder_get_object(data->builder, "edit_menu"),
		TRUE);
	gtk_widget_set_sensitive(
		(GtkWidget *)gtk_builder_get_object(data->builder, "app_view_pane"),
		TRUE);
	gtk_widget_set_sensitive(
		(GtkWidget *)gtk_builder_get_object(
			data->builder, "table_view_filter_item_selection_combobox"),
		FALSE);
}


static void refresh_after_db(CallBackData *data) {
	refresh_notebooks(data);
	refresh_prefs(data);
}


void init_all(CallBackData *data) {
	sqlite3_close(data->db);

	sqlite3 *db = NULL;
	if (!init_connection(&db, data)) {
		return;
	}
	if (!new_db(db, data)) {
		return;
	}
	init_db(db);

	data->db = db;
	data->meta = g_new(Meta, 1);
	*(data->meta) = Meta_from_db(db);

	init_core(data);
	init_prefs(data);
	init_notebooks(data);
	init_table_view(data);

	init_set_sensitive(data);
	refresh_after_db(data);
}


int main(int argc, char* argv[]) {
	gtk_init(&argc, &argv);

	GtkBuilder *builder = gtk_builder_new_from_string(gui_builder_string, -1);

	CallBackData cb_data = (CallBackData) {
		.db      = NULL,
		.builder = builder,
		.meta    = NULL,
		.db_name = NULL
	};

	GObject *window = gtk_builder_get_object(builder, "main_window");
	g_signal_connect(window, "destroy", G_CALLBACK(quit_CB), &cb_data);
	init_file_menu(&cb_data);
	init_help_menu(&cb_data);

	gtk_widget_show(GTK_WIDGET(window));
	gtk_main();
	return 0;
}

