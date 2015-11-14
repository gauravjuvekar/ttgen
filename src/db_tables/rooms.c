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

#include "rooms.h"


Room Room_from_stmt(sqlite3_stmt *stmt) {
	return (Room) {
		.pk       = sqlite3_column_int(stmt, 0),
		.name     = (const gchar*) sqlite3_column_text(stmt, 1),
		.capacity = sqlite3_column_int(stmt, 2)
	};
}


Room *Rooms_from_db(sqlite3 *db, const Meta *meta) {
	Room *rooms = g_malloc(sizeof(Room) * meta->n_rooms);

	sqlite3_stmt *stmt;
	gint sql_ret;
	sql_ret = sqlite3_prepare(
		db, "SELECT pk, name, capacity FROM rooms",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	gint i = 0;
	while ((sql_ret = sqlite3_step(stmt)) == SQLITE_ROW) {
		rooms[i] = Room_from_stmt(stmt);
		i++;
	}
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	return rooms;
}


void insert_Room(sqlite3 *db, const Room *room) {
	sqlite3_stmt *stmt;
	gint sql_ret;
	sql_ret = sqlite3_prepare(
		db, "INSERT INTO rooms(name, capacity) VALUES (:name, :capacity);",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_text(
		stmt, sqlite3_bind_parameter_index(stmt, ":name"),
		room->name, -1, SQLITE_STATIC);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":capacity"), room->capacity);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);
}


void remove_Room(sqlite3 *db, gint pk) {
	sqlite3_stmt *stmt;
	gint sql_ret;
	sql_ret = sqlite3_prepare(
		db, "DELETE FROM rooms WHERE pk=:pk;", -1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":pk"), pk);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);
}
