#include "population.h"

#include "db.h"

Population Population_from_db(sqlite3 *db, const Meta *meta) {
	Population population;
	population.generations = 0;
	population.schedules = g_ptr_array_sized_new(meta->n_population);
	g_ptr_array_set_free_func(population.schedules,
	                          (GDestroyNotify)Schedule_free);
	gint individual;
	for(individual = 0; individual < meta->n_population; individual++) {
		g_ptr_array_add(population.schedules,
		                Schedule_from_db(individual, db, meta));
	}

	return population;
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

	sql_ret = sqlite3_prepare(db, "DELETE FROM schedules;", -1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);

	extend_db_Population(population, db, meta);

	sql_ret = sqlite3_prepare(db, "COMMIT;", -1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);
}


void extend_db_Population(Population population,
                          sqlite3 *db, const Meta *meta) {
	sqlite3_stmt *stmt;
	gint sql_ret;
	gint schedule;
	for (schedule = 0; schedule < meta->n_population; schedule++) {
		insert_Schedule(g_ptr_array_index(population.schedules, schedule),
					    db, meta);
	}

	sql_ret = sqlite3_prepare(db,
	                          "INSERT OR REPLACE INTO meta(key, int_value) "
	                          "VALUES(\"n_generations\", :generations);",
	                          -1, &stmt, NULL);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_bind_int(
		stmt, sqlite3_bind_parameter_index(stmt, ":generations"),
		population.generations);
	g_assert(sql_ret == SQLITE_OK);
	sql_ret = sqlite3_step(stmt);
	g_assert(sql_ret == SQLITE_DONE);
	sql_ret = sqlite3_finalize(stmt);
	g_assert(sql_ret == SQLITE_OK);
}
