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
#include "schedule.h"

Schedule *Schedule_from_db(gint pk, sqlite3 *db, const Meta *meta) {
	Schedule *schedule = Schedule_init(meta);
	sqlite3_stmt *stmt;
	gint sql_ret;

	sql_ret = sqlite3_prepare(
		db, "SELECT allocation, slot FROM schedule_allocs WHERE schedule=:pk;",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":pk"), pk);
	g_assert(sql_ret == SQLITE_OK);
	while((sql_ret = sqlite3_step(stmt)) == SQLITE_ROW) {
		gint allocation;
		gint slot;

		allocation = sqlite3_column_int(stmt, 0);
		slot       = sqlite3_column_int(stmt, 1);
		g_assert(allocation < meta->n_allocs);
		g_assert(slot < meta->n_time_slots * meta->n_rooms);
		schedule->allocations[allocation] = slot;
		schedule->time_slots[slot] = allocation;
	}
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);


	sql_ret = sqlite3_prepare(
		db, "SELECT fitness FROM schedules WHERE pk=:pk;",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":pk"), pk);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_ROW);
	schedule->fitness = sqlite3_column_double(stmt, 0);

	return schedule;
}


void insert_Schedule(Schedule *schedule, sqlite3 *db, const Meta *meta) {
	gint sql_ret;
	sqlite3_stmt *stmt;
	sql_ret = sqlite3_prepare(
		db, "INSERT INTO schedules(fitness) VALUES(:fitness);",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_double(
		stmt,
		sqlite3_bind_parameter_index(stmt, ":fitness"),
		schedule->fitness);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);
	gint pk = sqlite3_last_insert_rowid(db);

	gint alloc;
	for (alloc = 0; alloc < meta->n_allocs; alloc++) {
		sql_ret = sqlite3_prepare(
			db,
			"INSERT INTO schedule_allocs(schedule, allocation, slot) "
			"VALUES(:schedule, :allocation, :slot);", -1, &stmt, NULL);
		g_assert(sql_ret == SQLITE_OK);
		sql_ret = sqlite3_bind_int(
			stmt, sqlite3_bind_parameter_index(stmt, ":schedule"), pk);
		g_assert(sql_ret == SQLITE_OK);
		sql_ret = sqlite3_bind_int(
			stmt, sqlite3_bind_parameter_index(stmt, ":allocation"), alloc);
		g_assert(sql_ret == SQLITE_OK);
		sql_ret = sqlite3_bind_int(
			stmt, sqlite3_bind_parameter_index(stmt, ":slot"),
			schedule->allocations[alloc]);
		g_assert(sql_ret == SQLITE_OK);
		sql_ret = sqlite3_step(stmt);
		g_assert(sql_ret == SQLITE_DONE);
		sqlite3_finalize(stmt);
	}
}
