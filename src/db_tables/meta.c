#include <glib.h>
#include "../sqlite/sqlite3.h"
#include "meta.h"


Meta Meta_from_db(sqlite3 *db) {
	sqlite3_stmt *stmt;
	Meta retval;
	Allocation *allocs;

	sqlite3_prepare(db, "SELECT float_value FROM meta "
	                    "WHERE key=\"fitness_penalty_time_clash_teacher\"",
	                -1, &stmt, NULL);
	sqlite3_step(stmt);
	retval.fitness_penalty_time_clash_teacher = sqlite3_column_double(stmt, 0);
	sqlite3_finalize(stmt);

	sqlite3_prepare(db, "SELECT float_value FROM meta "
	                    "WHERE key=\"fitness_penalty_time_clash_batch\"",
	                -1, &stmt, NULL);
	sqlite3_step(stmt);
	retval.fitness_penalty_time_clash_batch = sqlite3_column_double(stmt, 0);
	sqlite3_finalize(stmt);

	sqlite3_prepare(db, "SELECT int_value FROM meta "
	                    "WHERE key=\"n_time_slots_per_day\"",
	                -1, &stmt, NULL);
	sqlite3_step(stmt);
	retval.n_time_slots_per_day = sqlite3_column_int(stmt, 0);
	sqlite3_finalize(stmt);

	sqlite3_prepare(db, "SELECT int_value FROM meta WHERE key=\"n_time_slots\"",
	                -1, &stmt, NULL);
	sqlite3_step(stmt);
	retval.n_time_slots = sqlite3_column_int(stmt, 0);
	sqlite3_finalize(stmt);

	sqlite3_prepare(db, "SELECT int_value FROM meta WHERE key=\"mutate_swaps\"",
	                -1, &stmt, NULL);
	sqlite3_step(stmt);
	retval.mutate_swaps = sqlite3_column_int(stmt, 0);
	sqlite3_finalize(stmt);

	sqlite3_prepare(db, "SELECT int_value FROM meta WHERE key=\"n_population\"",
	                -1, &stmt, NULL);
	sqlite3_step(stmt);
	retval.n_population = sqlite3_column_int(stmt, 0);
	sqlite3_finalize(stmt);

	sqlite3_prepare(db, "SELECT COUNT(*) FROM teachers", -1, &stmt, NULL);
	sqlite3_step(stmt);
	retval.n_teachers = sqlite3_column_int(stmt, 0);
	sqlite3_finalize(stmt);

	sqlite3_prepare(db, "SELECT COUNT(*) FROM rooms", -1, &stmt, NULL);
	sqlite3_step(stmt);
	retval.n_rooms = sqlite3_column_int(stmt, 0);
	sqlite3_finalize(stmt);

	sqlite3_prepare(db, "SELECT COUNT(*) FROM batches", -1, &stmt, NULL);
	sqlite3_step(stmt);
	retval.n_batches = sqlite3_column_int(stmt, 0);
	sqlite3_finalize(stmt);

	sqlite3_prepare(db, "SELECT COUNT(*) FROM allocations", -1, &stmt, NULL);
	sqlite3_step(stmt);
	retval.n_allocs = sqlite3_column_int(stmt, 0);
	sqlite3_finalize(stmt);

	allocs = g_malloc(sizeof(Allocation) * retval.n_allocs);

	sqlite3_prepare(db, "SELECT batch, subject, teacher FROM allocations",
	                -1, &stmt, NULL);
	gint i = 0;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		allocs[i] = Allocation_from_stmt(stmt);
		i++;
	}
	sqlite3_finalize(stmt);
	retval.allocs = allocs;

	return retval;
}
