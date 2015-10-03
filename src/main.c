#include <gtk/gtk.h>

#include "notebook/rooms.h"

const char gui_builder_string[] = {
#include "gui.ui.hex.inc"
	, '\0'
};

int main(int argc, char* argv[]) {
	gtk_init(&argc, &argv);

	GtkBuilder *builder = gtk_builder_new_from_string(gui_builder_string, -1);
	GObject* window = gtk_builder_get_object(builder, "main_window");
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	GObject *rooms_tree_view = gtk_builder_get_object(builder,
	                                                  "rooms_tree_view");
	GtkListStore *list_store = Rooms_ListStore_new();
	set_Rooms_from_db(list_store);
	gtk_tree_view_set_model(GTK_TREE_VIEW(rooms_tree_view), GTK_TREE_MODEL(list_store));


	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;

	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("Name", renderer,
													  "text", COLUMN_STRING_name,
													  NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(rooms_tree_view), column);

	gtk_widget_show(GTK_WIDGET(window));
	gtk_main();
	return 0;
}

