#include "main.h"

#include "notebook/notebook.h"
#include "menubar/prefs.h"
#include "menubar/file.h"
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
		(GtkWidget *)gtk_builder_get_object(data->builder, "view_menu"),
		TRUE);
	gtk_widget_set_sensitive(
		(GtkWidget *)gtk_builder_get_object(data->builder, "app_view_pane"),
		TRUE);
}


static void refresh_after_db(CallBackData *data) {
	refresh_notebooks(data);
	refresh_prefs(data);
}


void init_all(CallBackData *data) {
	sqlite3 *db = NULL;
	init_connection(&db, data->db_name);
	new_db(db);
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

	gtk_widget_show(GTK_WIDGET(window));
	gtk_main();
	return 0;
}

