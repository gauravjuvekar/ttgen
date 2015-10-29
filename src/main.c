#include <gtk/gtk.h>

#include "notebook/notebook.h"

const char gui_builder_string[] = {
#include "gui.ui.hex.inc"
	, '\0'
};

const char db_schema_string[] = {
#include "database.sqlite3.hex.inc"
	, '\0'
};

int main(int argc, char* argv[]) {
	gtk_init(&argc, &argv);
	sqlite3 *db;
	sqlite3_open("new.db", &db);

	GtkBuilder *builder = gtk_builder_new_from_string(gui_builder_string, -1);
	GObject *window = gtk_builder_get_object(builder, "main_window");
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	CallBackData cb_data = (CallBackData) {
		.db = db,
		.builder = builder
	};
	init_notebooks(&cb_data);

	gtk_widget_show(GTK_WIDGET(window));
	gtk_main();
	return 0;
}

