#include "../sqlite/sqlite3.h"
#include "rooms.h"


Room Room_from_stmt(sqlite3_stmt *stmt) {
	return (Room) {
		.pk       = sqlite3_column_int(stmt, 0),
		.name     = (const gchar*) sqlite3_column_text(stmt, 1),
		.capacity = sqlite3_column_int(stmt, 2)
	};
}
