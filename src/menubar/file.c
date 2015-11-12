#include "file.h"
#include "../main.h"

static void file_new_CB(GtkWidget *widget, CallBackData *data) {
	(void)widget;
	GtkWidget *dialog;
	GtkWindow *window = (GtkWindow *)gtk_builder_get_object(
		data->builder, "main_window");
	dialog = gtk_file_chooser_dialog_new(
		"New File", window, GTK_FILE_CHOOSER_ACTION_SAVE,
		"Cancel", GTK_RESPONSE_CANCEL,
		"Create", GTK_RESPONSE_ACCEPT,
		NULL);

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
		gchar *filename = gtk_file_chooser_get_filename(
			GTK_FILE_CHOOSER(dialog));
		data->db_name = filename;
		init_all(data);
	}
	gtk_widget_destroy (dialog);
}


static void file_open_CB(GtkWidget *widget, CallBackData *data) {
	(void)widget;
	GtkWidget *dialog;
	GtkWindow *window = (GtkWindow *)gtk_builder_get_object(
		data->builder, "main_window");
	dialog = gtk_file_chooser_dialog_new(
		"New File", window, GTK_FILE_CHOOSER_ACTION_OPEN,
		"Cancel", GTK_RESPONSE_CANCEL,
		"Open", GTK_RESPONSE_ACCEPT,
		NULL);

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
		gchar *filename = gtk_file_chooser_get_filename(
			GTK_FILE_CHOOSER(dialog));
		data->db_name = filename;
		init_all(data);
	}
	gtk_widget_destroy (dialog);
}


void init_file_menu(CallBackData *data) {
	g_signal_connect(gtk_builder_get_object(data->builder,
	                                        "file_new_menuitem"),
	                 "activate", G_CALLBACK(file_new_CB), data);
	g_signal_connect(gtk_builder_get_object(data->builder,
	                                        "file_open_menuitem"),
	                 "activate", G_CALLBACK(file_open_CB), data);
	g_signal_connect(gtk_builder_get_object(data->builder,
	                                        "file_quit_menuitem"),
	                 "activate", G_CALLBACK(quit_CB), data);
}
