#include "../main.h"

#include "allocations.h"

Allocation Allocation_from_stmt(sqlite3_stmt *stmt) {
	return (Allocation) {
		.pk      = sqlite3_column_int(stmt, 0),
		.batch   = sqlite3_column_int(stmt, 1),
		.subject = sqlite3_column_int(stmt, 2),
		.teacher = sqlite3_column_int(stmt, 3)
	};
}


void insert_Allocation(sqlite3 *db, const Allocation *allocation) {
	sqlite3_stmt *stmt;
	g_assert(sqlite3_prepare(db,
	                         "INSERT INTO allocations(subject, teacher, batch) "
	                         "VALUES (:subject, :teacher, :batch);",
	                         -1, &stmt, NULL) == SQLITE_OK);
	g_assert(sqlite3_bind_int(stmt,
	                          sqlite3_bind_parameter_index(stmt, ":subject"),
	                          allocation->subject) == SQLITE_OK);
	g_assert(sqlite3_bind_int(stmt,
	                          sqlite3_bind_parameter_index(stmt, ":teacher"),
	                          allocation->teacher) == SQLITE_OK);
	g_assert(sqlite3_bind_int(stmt,
	                          sqlite3_bind_parameter_index(stmt, ":batch"),
	                          allocation->batch) == SQLITE_OK);
	g_assert(sqlite3_step(stmt) == SQLITE_DONE);
	g_assert(sqlite3_finalize(stmt) == SQLITE_OK);
}


void remove_Allocation(sqlite3 *db, gint pk) {
	sqlite3_stmt *stmt;
	g_assert(sqlite3_prepare(db,
	                         "DELETE FROM allocations WHERE pk=:pk;",
	                         -1, &stmt, NULL) == SQLITE_OK);
	g_assert(sqlite3_bind_int(stmt,
	                          sqlite3_bind_parameter_index(stmt, ":pk"), pk) ==
	         SQLITE_OK);
	g_assert(sqlite3_step(stmt) == SQLITE_DONE);
	g_assert(sqlite3_finalize(stmt) == SQLITE_OK);
}
