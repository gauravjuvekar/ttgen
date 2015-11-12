#include "../main.h"

#include "db.h"

static const char schema[] = {
#include "../database.sqlite3.hex.inc"
	, '\0'
};


void init_connection(sqlite3 **db, const char *file_name) {
	gint sql_ret;
	sqlite3_stmt *stmt;

	sql_ret = sqlite3_open(file_name, db);
	g_assert(sql_ret == SQLITE_OK);
	sqlite3_prepare(*db, "PRAGMA foreign_keys = ON;", -1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);
}


void init_db(sqlite3 *db) {
	Meta meta;
	meta.n_time_slots                       = 35;
	meta.n_time_slots_per_day               = 7;
	meta.mutate_swaps                       = 3;
	meta.fitness_penalty_time_clash_batch   = -1000;
	meta.fitness_penalty_time_clash_teacher = -1000;
	meta.n_population                       = 30;
	meta.db_schedules_valid                 = 1;

	init_db_with_Meta(db, &meta);
}


void new_db(sqlite3 *db) {
	sqlite3_stmt *stmt;
	gint sql_ret;

	gchar **sqls = g_strsplit(schema, ";", -1);
	gint i;
	for (i = 0; sqls[i] != NULL; i++) {
		if (g_strrstr(sqls[i], "CREATE") != NULL) {
			gchar *sql = g_strconcat(sqls[i], ";", NULL);
			sql_ret = sqlite3_prepare(db, sql, -1, &stmt, NULL);
			g_assert(sql_ret == SQLITE_OK);
			sql_ret = sqlite3_step(stmt);
			g_assert(sql_ret == SQLITE_DONE);
			sql_ret = sqlite3_finalize(stmt);
			g_assert(sql_ret == SQLITE_OK);
			g_free(sql);
		}
	}
	g_strfreev(sqls);
}


void reset_pks(sqlite3 *db) {
	sqlite3_stmt *stmt;

	const gchar *tables[] = {
		"batches",
		"teachers",
		"allocations",
		"rooms",
		"subjects",
		"schedules"
	};
	const gint tables_n = sizeof(tables) / sizeof(tables[0]);

	gint table;
	for (table = 0; table < tables_n; table++) {
		gchar *sql = g_strdup_printf(
			"UPDATE %s SET pk = (SELECT COUNT() FROM %s tmp "
			                     "WHERE tmp.pk < %s.pk);",
			tables[table], tables[table], tables[table]);
		gint sql_ret;
		sql_ret = sqlite3_prepare(db, sql, -1, &stmt, NULL);
		g_assert(sql_ret == SQLITE_OK);
		sql_ret = sqlite3_step(stmt);
		g_assert(sql_ret == SQLITE_DONE);
		sql_ret = sqlite3_finalize(stmt);
		g_assert(sql_ret == SQLITE_OK);
		g_free(sql);
	}
}
