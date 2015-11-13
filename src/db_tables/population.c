#include "../libs.h"
#include "population.h"
#include "schedule.h"

Population Population_from_db(sqlite3 *db, const Meta *meta) {
	Population population;
	population.schedules = g_ptr_array_sized_new(meta->n_population);
	g_ptr_array_set_free_func(population.schedules,
	                          (GDestroyNotify)Schedule_free);
	gint individual;
	for(individual = 0; individual < meta->n_population; individual++) {
		g_ptr_array_add(population.schedules,
		                Schedule_from_db(individual, db, meta));
	}
	population.n_schedules = individual;
	return population;
}


static void extend_db_Population(Population population,
                                 sqlite3 *db, const Meta *meta) {
	gint schedule;
	for (schedule = 0; schedule < population.n_schedules; schedule++) {
		insert_Schedule(g_ptr_array_index(population.schedules, schedule),
					    db, meta);
	}
}

void delete_db_Population(sqlite3 *db) {
	sqlite3_stmt *stmt;
	gint sql_ret;

	sql_ret = sqlite3_prepare(db, "DELETE FROM schedules;", -1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);
}

void replace_db_Population(Population population,
                           sqlite3 *db, const Meta *meta) {
	sqlite3_stmt *stmt;
	gint sql_ret;

	sql_ret = sqlite3_prepare(db, "BEGIN TRANSACTION;", -1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	delete_db_Population(db);
	extend_db_Population(population, db, meta);

	sql_ret = sqlite3_prepare(db, "COMMIT;", -1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);
}


void truncate_db_Population(gint schedules, sqlite3 *db) {
	gint sql_ret;
	sqlite3_stmt *stmt;

	sql_ret = sqlite3_prepare(
		db,
		"SELECT COUNT() FROM schedules;",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_ROW);
	gint current_schedules = sqlite3_column_int(stmt, 0);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	gint to_delete = current_schedules - schedules;

	sql_ret = sqlite3_prepare(
		db,
		"DELETE FROM schedules ORDER BY fitness LIMIT :difference;",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":difference"), to_delete);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);
}


void setup_population(gint schedules, sqlite3 *db,
                      const Meta *meta, const Allocation allocs[]) {
	gint sql_ret;
	sqlite3_stmt *stmt;
	sql_ret = sqlite3_prepare(
		db,
		"SELECT COUNT() FROM schedules;",
		-1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_ROW);
	gint current_schedules = sqlite3_column_int(stmt, 0);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	if (current_schedules > schedules) {
		gint to_delete = current_schedules - schedules;

		sql_ret = sqlite3_prepare(
			db,
			"DELETE FROM schedules WHERE pk NOT IN "
			"(SELECT pk FROM schedules ORDER BY fitness LIMIT :difference);",
			-1, &stmt, NULL);
		g_assert(sql_ret == SQLITE_OK);
		sql_ret = sqlite3_bind_int(
			stmt, sqlite3_bind_parameter_index(stmt, ":difference"), to_delete);
		g_assert(sql_ret == SQLITE_OK);
		sql_ret = sqlite3_step(stmt);
		g_assert(sql_ret == SQLITE_DONE);
		sql_ret = sqlite3_finalize(stmt);
		g_assert(sql_ret == SQLITE_OK);
	}
	else if (current_schedules < schedules) {
		Population population;
		Meta meta_copy = *meta;
		meta_copy.n_population = schedules - current_schedules;
		Population_seed(&population, &meta_copy, allocs);
		extend_db_Population(population, db, &meta_copy);
		Population_free(&population);
	}
}


