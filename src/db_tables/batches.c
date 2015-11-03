#include "../main.h"

#include "batches.h"

Batch Batch_from_stmt(sqlite3_stmt *stmt) {
	return (Batch) {
		.pk     = sqlite3_column_int(stmt, 0),
		.name   = (const gchar*) sqlite3_column_text(stmt, 1),
		.heads  = sqlite3_column_int(stmt, 2),
		.parent = sqlite3_column_int(stmt, 3)
	};
}


void insert_Batch(sqlite3 *db, const Batch *batch) {
	sqlite3_stmt *stmt;
	g_assert(sqlite3_prepare(db,
	                         "INSERT INTO batches(name, capacity) "
	                         "VALUES (:name, :heads, :parent);",
	                         -1, &stmt, NULL) == SQLITE_OK);
	g_assert(sqlite3_bind_text(stmt,
	                           sqlite3_bind_parameter_index(stmt, ":name"),
	                           batch->name, -1, SQLITE_STATIC) == SQLITE_OK);
	g_assert(sqlite3_bind_int(stmt,
	                           sqlite3_bind_parameter_index(stmt, ":heads"),
	                           batch->heads) == SQLITE_OK);
	g_assert(sqlite3_bind_int(stmt,
	                           sqlite3_bind_parameter_index(stmt, ":parent"),
	                           batch->parent) == SQLITE_OK);
	g_assert(sqlite3_step(stmt) == SQLITE_DONE);
	g_assert(sqlite3_finalize(stmt) == SQLITE_OK);
}


void remove_Batch(sqlite3 *db, gint pk) {
	sqlite3_stmt *stmt;
	g_assert(sqlite3_prepare(db,
	                         "DELETE FROM batches WHERE pk=:pk;",
	                         -1, &stmt, NULL) == SQLITE_OK);
	g_assert(sqlite3_bind_int(stmt,
	                          sqlite3_bind_parameter_index(stmt, ":pk"), pk) ==
	         SQLITE_OK);
	g_assert(sqlite3_step(stmt) == SQLITE_DONE);
	g_assert(sqlite3_finalize(stmt) == SQLITE_OK);
}
