#include "../main.h"

#include "teachers.h"


Teacher Teacher_from_stmt(sqlite3_stmt *stmt) {
	return (Teacher) {
		.pk   = sqlite3_column_int(stmt, 0),
		.name = (const gchar*) sqlite3_column_text(stmt, 1),
	};
}


void insert_Teacher(sqlite3 *db, const Teacher *teacher) {
	sqlite3_stmt *stmt;
	g_assert(sqlite3_prepare(db,
	                         "INSERT INTO teachers(name) VALUES (:name);",
	                         -1, &stmt, NULL) == SQLITE_OK);
	g_assert(sqlite3_bind_text(stmt,
	                           sqlite3_bind_parameter_index(stmt, ":name"),
	                           teacher->name, -1, SQLITE_STATIC) == SQLITE_OK);
	g_assert(sqlite3_step(stmt) == SQLITE_DONE);
	g_assert(sqlite3_finalize(stmt) == SQLITE_OK);
}


void remove_Teacher(sqlite3 *db, gint pk) {
	sqlite3_stmt *stmt;
	g_assert(sqlite3_prepare(db,
	                         "DELETE FROM teachers WHERE pk=:pk;",
	                         -1, &stmt, NULL) == SQLITE_OK);
	g_assert(sqlite3_bind_int(stmt,
	                          sqlite3_bind_parameter_index(stmt, ":pk"), pk) ==
	         SQLITE_OK);
	g_assert(sqlite3_step(stmt) == SQLITE_DONE);
	g_assert(sqlite3_finalize(stmt) == SQLITE_OK);
}
