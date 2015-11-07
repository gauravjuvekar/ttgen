#include "../main.h"

#include "db.h"

static const char schema[] = {
#include "../database.sqlite3.hex.inc"
	, '\0'
};

void init_db(sqlite3 *db, const char *file_name) {
	g_assert(sqlite3_open(file_name, &db) == SQLITE_OK);
	sqlite3_stmt *stmt;
	g_assert(sqlite3_prepare(db, "PRAGMA foreign_keys = ON;",
	                         -1, &stmt, NULL) == SQLITE_OK);
	g_assert(sqlite3_step(stmt) == SQLITE_DONE);
	g_assert(sqlite3_finalize(stmt) == SQLITE_OK);
}

void new_db(sqlite3 *db) {
	sqlite3_stmt *stmt;
	g_assert(sqlite3_prepare(db, schema, -1, &stmt, NULL) == SQLITE_OK);
	g_assert(sqlite3_step(stmt) == SQLITE_OK);
	g_assert(sqlite3_finalize(stmt) == SQLITE_OK);
}

void reset_pks(sqlite3 *db) {
	sqlite3_stmt *stmt;

	const gchar *tables[] = {
		"batches",
		"teachers",
		"allocations",
		"rooms",
		"subjects"
	};
	const gint tables_n = sizeof(tables) / sizeof(tables[0]);

	gint table;
	for (table = 0; table < tables_n; table++) {
		gchar *sql = g_strdup_printf(
			"UPDATE %s SET pk = (SELECT COUNT() FROM %s tmp "
			                     "WHERE tmp.pk < %s.pk);",
			tables[table], tables[table], tables[table]);
		g_assert(sqlite3_prepare(db, sql, -1, &stmt, NULL) == SQLITE_OK);
		g_assert(sqlite3_step(stmt) == SQLITE_DONE);
		g_assert(sqlite3_finalize(stmt) == SQLITE_OK);
		g_free(sql);
	}
}
