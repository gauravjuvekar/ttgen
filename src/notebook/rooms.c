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

#include "rooms.h"
#include "../db_tables/db.h"


typedef enum {
	COLUMN_INT_pk,
	COLUMN_STRING_name,
	COLUMN_INT_capacity,
	N_COLUMNS
} TreeView_Rooms_E;

static void set_Rooms_from_db(GtkListStore *list_store, sqlite3 *db);


static void add_room_CB(GtkButton* button, CallBackData *data) {
	(void)button;
	GObject *name_entry = gtk_builder_get_object(
		data->builder, "rooms_add_window_name_entry");
	GObject *capacity_spin_button = gtk_builder_get_object(
		data->builder, "rooms_add_window_capacity_spinbutton");
	Room room = (Room) {
		.name = gtk_entry_get_text((GtkEntry *)name_entry),
		.capacity = gtk_spin_button_get_value_as_int(
			 (GtkSpinButton *)capacity_spin_button)
	};

	insert_Room(data->db, &room);
	data->meta->n_rooms += 1;
	GObject *window = gtk_builder_get_object(
		data->builder, "rooms_add_window");
	gtk_widget_hide((GtkWidget *)window);
	gtk_entry_set_text((GtkEntry *)name_entry, "");

	/* Because the number of slots change */
	sqlite3_stmt *stmt;
	gint sql_ret;
	sql_ret = sqlite3_prepare(
		data->db,
		"INSERT OR REPLACE INTO meta(key, int_value) "
		"VALUES(\"db_schedules_valid\", :schedules_valid);",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":schedules_valid"),
		0);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	refresh_notebook_rooms(data);
}


static void cancel_add_room_CB(GtkButton* button, CallBackData *data) {
	(void)button;
	GObject *name_entry = gtk_builder_get_object(
		data->builder, "rooms_add_window_name_entry");
	GObject *window = gtk_builder_get_object(
		data->builder, "rooms_add_window");
	gtk_widget_hide((GtkWidget *)window);
	gtk_entry_set_text((GtkEntry *)name_entry, "");
}


static gboolean close_add_room_window_CB(GtkWidget *widget,
                                         GdkEvent  *event,
                                         CallBackData *data) {
	(void)widget;
	(void)event;
	cancel_add_room_CB(NULL, data);
	return TRUE;
}


void init_notebook_rooms(CallBackData *data) {
	GtkTreeView *rooms_tree_view = GTK_TREE_VIEW(
		gtk_builder_get_object(data->builder, "rooms_tree_view"));

	gtk_tree_view_append_column(
	  	rooms_tree_view,
	 	gtk_tree_view_column_new_with_attributes(
	 		"Name",
	 		gtk_cell_renderer_text_new(),
	 		"text", COLUMN_STRING_name,
	  		NULL));
	gtk_tree_view_append_column(
		rooms_tree_view,
		gtk_tree_view_column_new_with_attributes(
			"Capacity",
			gtk_cell_renderer_text_new(),
			"text", COLUMN_INT_capacity,
			NULL));


	GObject *add_room_button = gtk_builder_get_object(
		data->builder, "rooms_add_window_ok_button");
	g_signal_connect(add_room_button, "clicked",
	                 G_CALLBACK(add_room_CB), data);
	GObject *cancel_add_room_button = gtk_builder_get_object(
		data->builder, "rooms_add_window_cancel_button");
	g_signal_connect(cancel_add_room_button, "clicked",
	                 G_CALLBACK(cancel_add_room_CB), data);
	GObject *add_room_window = gtk_builder_get_object(data->builder,
	                                                  "rooms_add_window");
	g_signal_connect(add_room_window, "delete-event",
	                 G_CALLBACK(close_add_room_window_CB), data);
}


static void set_Rooms_from_db(GtkListStore *list_store, sqlite3 *db) {
	sqlite3_stmt *stmt;
	sqlite3_prepare(db, "SELECT pk, name, capacity FROM rooms;",
	                -1, &stmt, NULL);
	GtkTreeIter iter;
	while(sqlite3_step(stmt) == SQLITE_ROW) {
		gtk_list_store_append(list_store, &iter);
		Room room = Room_from_stmt(stmt);
		gtk_list_store_set(list_store, &iter,
		                   COLUMN_INT_pk,       room.pk,
		                   COLUMN_STRING_name,  room.name,
		                   COLUMN_INT_capacity, room.capacity,
		                   -1);
		g_free((gpointer)room.name);
	}
	sqlite3_finalize(stmt);
}


void refresh_notebook_rooms(CallBackData *data) {
	GtkTreeView *rooms_tree_view = GTK_TREE_VIEW(
		gtk_builder_get_object(data->builder, "rooms_tree_view"));
	GtkListStore *list_store =
		(GtkListStore *)gtk_tree_view_get_model(rooms_tree_view);
	gtk_list_store_clear(list_store);
	set_Rooms_from_db(list_store, data->db);
}
