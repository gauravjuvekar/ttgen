/*  TTgen: An application to generate timetables using a genetic algorithm.
 *  Copyright (C) 2015  Gaurav Juvekar
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "../libs.h"

#include "db.h"

static const char schema[] = {
#include "../database.sqlite3.hex.inc"
	, '\0'
};

void init_failure_dialog(CallBackData *data) {
	GtkMessageDialog *dialog = (GtkMessageDialog *)gtk_message_dialog_new(
		(GtkWindow *)gtk_builder_get_object(data->builder, "main_window"),
		GTK_DIALOG_DESTROY_WITH_PARENT,
		GTK_MESSAGE_ERROR,
		GTK_BUTTONS_CLOSE,
		"Unable to open file %s\n"
		"Please ensure that it was created by this program.",
		data->db_name);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy((GtkWidget *)dialog);
}


gboolean init_connection(sqlite3 **db, CallBackData *data) {
	gint sql_ret;
	sqlite3_stmt *stmt;

	sql_ret = sqlite3_open(data->db_name, db);
	if (sql_ret != SQLITE_OK) {
		init_failure_dialog(data);
		return FALSE;
	}
	g_assert(sql_ret == SQLITE_OK);
	sqlite3_prepare(*db, "PRAGMA foreign_keys = ON;", -1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	if (sql_ret != SQLITE_DONE) {
		init_failure_dialog(data);
		sql_ret = sqlite3_finalize(stmt);
		g_assert(sql_ret == SQLITE_OK);
		return FALSE;
	}
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);
	return TRUE;
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


gboolean new_db(sqlite3 *db, CallBackData *data) {
	sqlite3_stmt *stmt;
	gint sql_ret;

	gchar **sqls = g_strsplit(schema, ";", -1);
	gint i;
	for (i = 0; sqls[i] != NULL; i++) {
		if (g_strrstr(sqls[i], "CREATE") != NULL) {
			gchar *sql = g_strconcat(sqls[i], ";", NULL);
			sql_ret = sqlite3_prepare(db, sql, -1, &stmt, NULL);
			if (sql_ret == SQLITE_NOTADB) {
				init_failure_dialog(data);
				sql_ret = sqlite3_finalize(stmt);
				g_assert(sql_ret == SQLITE_OK);
				g_free(sql);
				g_strfreev(sqls);
				return FALSE;
			}
			g_assert(sql_ret == SQLITE_OK);
			sql_ret = sqlite3_step(stmt);
			if (sql_ret != SQLITE_DONE) {
				init_failure_dialog(data);
				sql_ret = sqlite3_finalize(stmt);
				g_assert(sql_ret == SQLITE_OK);
				g_free(sql);
				g_strfreev(sqls);
				return FALSE;
			}
			g_assert(sql_ret == SQLITE_DONE);
			sql_ret = sqlite3_finalize(stmt);
			g_assert(sql_ret == SQLITE_OK);
			g_free(sql);
		}
	}
	g_strfreev(sqls);
	return TRUE;
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
