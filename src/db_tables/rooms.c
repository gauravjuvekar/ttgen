#include "../main.h"

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
	g_assert(sqlite3_prepare(db,
	                         "INSERT INTO rooms(name, capacity) "
	                         "VALUES (:name, :capacity);",
	                         -1, &stmt, NULL) == SQLITE_OK);
	g_assert(sqlite3_bind_text(stmt,
	                           sqlite3_bind_parameter_index(stmt, ":name"),
	                           room->name, -1, SQLITE_STATIC) == SQLITE_OK);
	g_assert(sqlite3_bind_int(stmt,
	                           sqlite3_bind_parameter_index(stmt, ":capacity"),
	                           room->capacity) == SQLITE_OK);
	g_assert(sqlite3_step(stmt) == SQLITE_DONE);
	g_assert(sqlite3_finalize(stmt) == SQLITE_OK);
}
