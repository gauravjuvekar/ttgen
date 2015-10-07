#include "../sqlite/sqlite3.h"
#include "subjects.h"


Subject Subject_from_stmt(sqlite3_stmt *stmt) {
	return (Subject) {
		.pk              = sqlite3_column_int(stmt, 0),
		.name            = (const gchar*) sqlite3_column_text(stmt, 1),
		.frequency       = sqlite3_column_int(stmt, 2),
		.contiguous_time = sqlite3_column_int(stmt, 3),
		.parallel        = sqlite3_column_int(stmt, 4)
	};
}
