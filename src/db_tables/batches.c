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
	gint sql_ret;
	sql_ret = sqlite3_prepare(
		db, "INSERT INTO batches(name, heads) VALUES (:name, :heads);",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_text(
		stmt, sqlite3_bind_parameter_index(stmt, ":name"),
		batch->name, -1, SQLITE_STATIC);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":heads"), batch->heads);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);
}


void remove_Batch(sqlite3 *db, gint pk) {
	sqlite3_stmt *stmt;
	gint sql_ret;
	sql_ret = sqlite3_prepare(
		db, "DELETE FROM batches WHERE pk=:pk;", -1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":pk"), pk);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);
}
