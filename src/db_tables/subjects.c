#include "../sqlite/sqlite3.h"
#include "subjects.h"


Subject Subject_from_stmt(sqlite3_stmt *stmt) {
	return (Subject) {
		.pk   = sqlite3_column_int(stmt, 0),
		.name = (const gchar*) sqlite3_column_text(stmt, 1)
	};
}


void insert_Subject(sqlite3 *db, const Subject *subject) {
	sqlite3_stmt *stmt;
	sqlite3_prepare(db, "INSERT INTO subjects(name), VALUES (:name)",
	                -1, &stmt, NULL);
	sqlite3_bind_text(stmt,
	                  sqlite3_bind_parameter_index(stmt, ":name"),
	                  subject->name, -1, SQLITE_STATIC);
	g_assert(sqlite3_step(stmt) == SQLITE_OK);
	sqlite3_finalize(stmt);
}
