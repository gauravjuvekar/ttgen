#include "../main.h"

#include "allocations.h"
#include "db.h"

Allocation Allocation_from_stmt(sqlite3_stmt *stmt) {
	return (Allocation) {
		.pk      = sqlite3_column_int(stmt, 0),
		.batch   = sqlite3_column_int(stmt, 1),
		.subject = sqlite3_column_int(stmt, 2),
		.teacher = sqlite3_column_int(stmt, 3)
	};
}

Allocation *Allocations_from_db(sqlite3 *db, const Meta *meta) {
	Allocation *allocs = g_malloc(sizeof(Allocation) * meta->n_allocs);

	sqlite3_stmt *stmt;
	gint sql_ret;
	sql_ret = sqlite3_prepare(
		db, "SELECT batch, subject, teacher FROM allocations",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	gint i = 0;
	while ((sql_ret = sqlite3_step(stmt)) == SQLITE_ROW) {
		allocs[i] = Allocation_from_stmt(stmt);
		i++;
	}
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	return allocs;
}


void insert_Allocation(sqlite3 *db, const Allocation *allocation) {
	sqlite3_stmt *stmt;
	gint sql_ret;
	sql_ret = sqlite3_prepare(
		db,
		"INSERT INTO allocations(subject, teacher, batch) "
		"VALUES (:subject, :teacher, :batch);",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":subject"),
		allocation->subject);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":teacher"),
		allocation->teacher);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":batch"), allocation->batch);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);
}


void remove_Allocation(sqlite3 *db, gint pk) {
	sqlite3_stmt *stmt;
	gint sql_ret;
	sql_ret = sqlite3_prepare(
		db, "DELETE FROM allocations WHERE pk=:pk;", -1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":pk"), pk);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);
}
