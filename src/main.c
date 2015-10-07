#include <gtk/gtk.h>

#include "notebook/rooms.h"
#include "notebook/subjects.h"

const char gui_builder_string[] = {
#include "gui.ui.hex.inc"
	, '\0'
};

int main(int argc, char* argv[]) {
	gtk_init(&argc, &argv);
	sqlite3 *db;
	sqlite3_open("database.db", &db);

	GtkBuilder *builder = gtk_builder_new_from_string(gui_builder_string, -1);
	GObject *window = gtk_builder_get_object(builder, "main_window");
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	init_notebook_rooms(builder, db);
	init_notebook_subjects(builder, db);


	gtk_widget_show(GTK_WIDGET(window));
	gtk_main();
	return 0;
}

