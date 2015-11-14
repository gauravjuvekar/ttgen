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

#include "meta.h"
#include "allocations.h"


Meta Meta_from_db(sqlite3 *db) {
	sqlite3_stmt *stmt;
	gint sql_ret;
	Meta retval;

	sql_ret = sqlite3_prepare(
		db,
		"SELECT float_value FROM meta "
		"WHERE key=\"fitness_penalty_time_clash_teacher\"",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_ROW);
	retval.fitness_penalty_time_clash_teacher = sqlite3_column_double(stmt, 0);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	sql_ret = sqlite3_prepare(
		db,
		"SELECT float_value FROM meta "
		"WHERE key=\"fitness_penalty_time_clash_batch\"",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_ROW);
	retval.fitness_penalty_time_clash_batch = sqlite3_column_double(stmt, 0);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	sql_ret = sqlite3_prepare(
		db,
		"SELECT float_value FROM meta "
		"WHERE key=\"fitness_penalty_room_capacity\"",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_ROW);
	retval.fitness_penalty_room_capacity = sqlite3_column_double(stmt, 0);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	sql_ret = sqlite3_prepare(
		db,
		"SELECT int_value FROM meta "
		"WHERE key=\"db_schedules_valid\"",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_ROW);
	retval.db_schedules_valid = sqlite3_column_int(stmt, 0);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	sql_ret = sqlite3_prepare(
		db,
		"SELECT int_value FROM meta "
		"WHERE key=\"n_time_slots_per_day\"",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_ROW);
	retval.n_time_slots_per_day = sqlite3_column_int(stmt, 0);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	sql_ret = sqlite3_prepare(
		db, "SELECT int_value FROM meta WHERE key=\"n_time_slots\"",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_ROW);
	retval.n_time_slots = sqlite3_column_int(stmt, 0);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	sql_ret = sqlite3_prepare(
		db, "SELECT int_value FROM meta WHERE key=\"mutate_swaps\"",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_ROW);
	retval.mutate_swaps = sqlite3_column_int(stmt, 0);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	sql_ret = sqlite3_prepare(
		db, "SELECT int_value FROM meta WHERE key=\"n_population\"",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_ROW);
	retval.n_population = sqlite3_column_int(stmt, 0);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	sql_ret = sqlite3_prepare(
		db, "SELECT COUNT(*) FROM teachers", -1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_ROW);
	retval.n_teachers = sqlite3_column_int(stmt, 0);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	sql_ret = sqlite3_prepare(
		db, "SELECT COUNT(*) FROM rooms", -1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_ROW);
	retval.n_rooms = sqlite3_column_int(stmt, 0);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	sql_ret = sqlite3_prepare(
		db, "SELECT COUNT(*) FROM batches", -1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_ROW);
	retval.n_batches = sqlite3_column_int(stmt, 0);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	sql_ret = sqlite3_prepare(
		db, "SELECT COUNT(*) FROM allocations", -1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_ROW);
	retval.n_allocs = sqlite3_column_int(stmt, 0);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);


	return retval;
}


void insert_Meta(sqlite3 *db, const Meta *meta) {
	sqlite3_stmt *stmt;
	gint sql_ret;

	sql_ret = sqlite3_prepare(
		db,
		"INSERT OR REPLACE INTO meta(key, int_value) "
		"VALUES(\"db_schedules_valid\", :schedules_valid);",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":schedules_valid"),
		meta->db_schedules_valid);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	sql_ret = sqlite3_prepare(
		db,
		"INSERT OR REPLACE INTO meta(key, int_value) "
		"VALUES(\"n_time_slots\", :time_slots);",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":time_slots"),
		meta->n_time_slots);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	sql_ret = sqlite3_prepare(
		db,
		"INSERT OR REPLACE INTO meta(key, int_value) "
		"VALUES(\"n_time_slots_per_day\", :time_slots);",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":time_slots"),
		meta->n_time_slots_per_day);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	sql_ret = sqlite3_prepare(
		db,
		"INSERT OR REPLACE INTO meta(key, int_value) "
		"VALUES(\"n_population\", :population);",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":population"),
		meta->n_population);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	sql_ret = sqlite3_prepare(
		db,
		"INSERT OR REPLACE INTO meta(key, int_value) "
		"VALUES(\"mutate_swaps\", :mutations);",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":mutations"),
		meta->mutate_swaps);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	sql_ret = sqlite3_prepare(
		db,
		"INSERT OR REPLACE INTO meta(key, float_value) "
		"VALUES(\"fitness_penalty_time_clash_batch\", :penalty);",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_double(
		stmt, sqlite3_bind_parameter_index(stmt, ":penalty"),
		meta->fitness_penalty_time_clash_batch);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	sql_ret = sqlite3_prepare(
		db,
		"INSERT OR REPLACE INTO meta(key, float_value) "
		"VALUES(\"fitness_penalty_time_clash_teacher\", :penalty);",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_double(
		stmt, sqlite3_bind_parameter_index(stmt, ":penalty"),
		meta->fitness_penalty_time_clash_teacher);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	sql_ret = sqlite3_prepare(
		db,
		"INSERT OR REPLACE INTO meta(key, float_value) "
		"VALUES(\"fitness_penalty_room_capacity\", :penalty);",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_double(
		stmt, sqlite3_bind_parameter_index(stmt, ":penalty"),
		meta->fitness_penalty_room_capacity);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);
}


void init_db_with_Meta(sqlite3 *db, const Meta *meta) {
	sqlite3_stmt *stmt;
	gint sql_ret;

	sql_ret = sqlite3_prepare(
		db,
		"INSERT OR IGNORE INTO meta(key, int_value) "
		"VALUES(\"db_schedules_valid\", :schedules_valid);",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":schedules_valid"),
		meta->db_schedules_valid);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	sql_ret = sqlite3_prepare(
		db,
		"INSERT OR IGNORE INTO meta(key, int_value) "
		"VALUES(\"n_time_slots\", :time_slots);",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":time_slots"),
		meta->n_time_slots);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	sql_ret = sqlite3_prepare(
		db,
		"INSERT OR IGNORE INTO meta(key, int_value) "
		"VALUES(\"n_time_slots_per_day\", :time_slots);",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":time_slots"),
		meta->n_time_slots_per_day);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	sql_ret = sqlite3_prepare(
		db,
		"INSERT OR IGNORE INTO meta(key, int_value) "
		"VALUES(\"n_population\", :population);",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":population"),
		meta->n_population);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	sql_ret = sqlite3_prepare(
		db,
		"INSERT OR IGNORE INTO meta(key, int_value) "
		"VALUES(\"mutate_swaps\", :mutations);",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":mutations"),
		meta->mutate_swaps);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	sql_ret = sqlite3_prepare(
		db,
		"INSERT OR IGNORE INTO meta(key, float_value) "
		"VALUES(\"fitness_penalty_time_clash_batch\", :penalty);",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_double(
		stmt, sqlite3_bind_parameter_index(stmt, ":penalty"),
		meta->fitness_penalty_time_clash_batch);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	sql_ret = sqlite3_prepare(
		db,
		"INSERT OR IGNORE INTO meta(key, float_value) "
		"VALUES(\"fitness_penalty_time_clash_teacher\", :penalty);",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_double(
		stmt, sqlite3_bind_parameter_index(stmt, ":penalty"),
		meta->fitness_penalty_time_clash_teacher);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	sql_ret = sqlite3_prepare(
		db,
		"INSERT OR IGNORE INTO meta(key, float_value) "
		"VALUES(\"fitness_penalty_room_capacity\", :penalty);",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_double(
		stmt, sqlite3_bind_parameter_index(stmt, ":penalty"),
		meta->fitness_penalty_room_capacity);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);
}
