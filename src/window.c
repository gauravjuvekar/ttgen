#include <gtk/gtk.h>

const char gui_builder_string[] = {
#include "gui.ui.hex.in"
	, '\0'
};

int main(int argc, char* argv[]) {
	gtk_init(&argc, &argv);

	GtkBuilder *builder = gtk_builder_new_from_string(gui_builder_string, -1);
	GObject* window = gtk_builder_get_object(builder, "main_window");
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_widget_show(GTK_WIDGET(window));
	gtk_main();
	return 0;
}

