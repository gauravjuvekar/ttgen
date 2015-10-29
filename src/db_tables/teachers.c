#include "../main.h"

#include "teachers.h"


Teacher Teacher_from_stmt(sqlite3_stmt *stmt) {
	return (Teacher) {
		.pk       = sqlite3_column_int(stmt, 0),
		.name     = (const gchar*) sqlite3_column_text(stmt, 1),
	};
}
