#include "../main.h"

#include "teachers.h"
#include "../db_tables/teachers.h"

typedef enum {
	COLUMN_INT_pk,
	COLUMN_STRING_name,
	N_COLUMNS
} TreeView_Teachers_E;

static GtkListStore *Teachers_ListStore_new(void);
static void set_Teachers_from_db(GtkListStore *list_store, sqlite3 *db);

static GtkListStore *Teachers_ListStore_new(void) {
	return gtk_list_store_new(N_COLUMNS,
	                          G_TYPE_INT,
	                          G_TYPE_STRING);
}

static void add_teacher_CB(GtkButton* button, CallBackData *data) {
	(void)button;
	GObject *name_entry = gtk_builder_get_object(
		data->builder, "teachers_add_window_name_entry");
	Teacher teacher = (Teacher) {
		.name = gtk_entry_get_text((GtkEntry *)name_entry)
	};

	insert_Teacher(data->db, &teacher);
	GObject *window = gtk_builder_get_object(
		data->builder, "teachers_add_window");
	gtk_widget_hide((GtkWidget *)window);
	gtk_entry_set_text((GtkEntry *)name_entry, "");

	refresh_notebook_teachers(data);
}


static void cancel_add_teacher_CB(GtkButton* button, CallBackData *data) {
	(void)button;
	GObject *name_entry = gtk_builder_get_object(
		data->builder, "teachers_add_window_name_entry");
	GObject *window = gtk_builder_get_object(
		data->builder, "teachers_add_window");
	gtk_widget_hide((GtkWidget *)window);
	gtk_entry_set_text((GtkEntry *)name_entry, "");
}


static gboolean close_add_teacher_window_CB(GtkWidget *widget,
                                            GdkEvent  *event,
                                            CallBackData *data) {
	(void)widget;
	(void)event;
	cancel_add_teacher_CB(NULL, data);
	return TRUE;
}


void init_notebook_teachers(CallBackData *data) {
	GtkTreeView *teachers_tree_view = GTK_TREE_VIEW(
		gtk_builder_get_object(data->builder, "teachers_tree_view")
	);
	GtkListStore *list_store = Teachers_ListStore_new();
	set_Teachers_from_db(list_store, data->db);
	gtk_tree_view_set_model(teachers_tree_view, GTK_TREE_MODEL(list_store));

	gtk_tree_view_append_column(
		teachers_tree_view,
		gtk_tree_view_column_new_with_attributes(
			"Name",
			gtk_cell_renderer_text_new(), "text", COLUMN_STRING_name, NULL)
	);

	GObject *add_teacher_button = gtk_builder_get_object(
		data->builder, "teachers_add_window_ok_button");
	g_signal_connect(add_teacher_button, "clicked",
	                 G_CALLBACK(add_teacher_CB), data);
	GObject *cancel_add_teacher_button = gtk_builder_get_object(
		data->builder, "teachers_add_window_cancel_button");
	g_signal_connect(cancel_add_teacher_button, "clicked",
	                 G_CALLBACK(cancel_add_teacher_CB), data);
	GObject *add_teacher_window = gtk_builder_get_object(data->builder,
													   "teachers_add_window");
	g_signal_connect(add_teacher_window, "delete-event",
	                 G_CALLBACK(close_add_teacher_window_CB), data);
}

static void set_Teachers_from_db(GtkListStore *list_store, sqlite3 *db) {
	sqlite3_stmt *stmt;
	sqlite3_prepare(db, "SELECT pk, name FROM teachers;", -1, &stmt, NULL);
	GtkTreeIter iter;
	while(sqlite3_step(stmt) == SQLITE_ROW) {
		gtk_list_store_append(list_store, &iter);
		Teacher teacher = Teacher_from_stmt(stmt);
		gtk_list_store_set(list_store, &iter,
		                   COLUMN_INT_pk,       teacher.pk,
		                   COLUMN_STRING_name,  teacher.name,
		                   -1);
	}
	sqlite3_finalize(stmt);
}

void refresh_notebook_teachers(CallBackData *data) {
	GtkTreeView *teachers_tree_view = GTK_TREE_VIEW(
		gtk_builder_get_object(data->builder, "teachers_tree_view"));
	GtkTreeModel *list_store = gtk_tree_view_get_model(teachers_tree_view);
	gtk_list_store_clear((GtkListStore *)list_store);
	set_Teachers_from_db((GtkListStore *)list_store, data->db);
}
