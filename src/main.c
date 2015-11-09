#include "main.h"

#include "db_tables/db.h"
#include "notebook/notebook.h"
#include "prefs/prefs.h"
#include "core/interface.h"

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
	sqlite3 *db = NULL;
	init_connection(&db, "new.db");
	init_db(db);

	GtkBuilder *builder = gtk_builder_new_from_string(gui_builder_string, -1);
	GObject *window = gtk_builder_get_object(builder, "main_window");
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	CallBackData cb_data = (CallBackData) {
		.db = db,
		.builder = builder
	};

	init_core(&cb_data);
	init_prefs(&cb_data);
	init_notebooks(&cb_data);

	gtk_widget_show(GTK_WIDGET(window));
	gtk_main();
	return 0;
}

