#include "../libs.h"

#include "subjects.h"


Subject Subject_from_stmt(sqlite3_stmt *stmt) {
	return (Subject) {
		.pk   = sqlite3_column_int(stmt, 0),
		.name = (const gchar*) sqlite3_column_text(stmt, 1)
	};
}


void insert_Subject(sqlite3 *db, const Subject *subject) {
	sqlite3_stmt *stmt;
	gint sql_ret;
	sql_ret = sqlite3_prepare(
		db, "INSERT INTO subjects(name) VALUES (:name);", -1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_text(
		stmt, sqlite3_bind_parameter_index(stmt, ":name"),
		subject->name, -1, SQLITE_STATIC);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);
}


void remove_Subject(sqlite3 *db, gint pk) {
	sqlite3_stmt *stmt;
	gint sql_ret;
	sql_ret = sqlite3_prepare(
		db, "DELETE FROM subjects WHERE pk=:pk;", -1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":pk"), pk);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);
}
