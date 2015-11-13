#include "../libs.h"

#include "rooms.h"


Room Room_from_stmt(sqlite3_stmt *stmt) {
	return (Room) {
		.pk       = sqlite3_column_int(stmt, 0),
		.name     = (const gchar*) sqlite3_column_text(stmt, 1),
		.capacity = sqlite3_column_int(stmt, 2)
	};
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
