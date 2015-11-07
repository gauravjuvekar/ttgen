#include <glib.h>
#include "../sqlite/sqlite3.h"
#include "meta.h"

Allocation Allocation_from_stmt(sqlite3_stmt *stmt) {
	return (Allocation) {
		.batch    = sqlite3_column_int(stmt, 1),
		.subject  = sqlite3_column_int(stmt, 2),
		.teacher  = sqlite3_column_int(stmt, 3)
	};
}

Meta Meta_from_db(sqlite3 *db) {
	sqlite3_stmt *stmt;
	gint n_teachers;
	gint n_batches;
	gint n_time_slots = 7;
	gint n_rooms;
	gint n_allocs;
	Allocation *allocs;
	gint mutate_swaps = 3;
	gfloat fitness_penalty_time_clash_teacher = -100;
	gfloat fitness_penalty_time_clash_batch = -100;
	gint n_population = 30;

	sqlite3_prepare(db, "SELECT COUNT(*) FROM teachers", -1, &stmt, NULL);
	sqlite3_step(stmt);
	n_teachers = sqlite3_column_int(stmt, 0);
	sqlite3_finalize(stmt);

	sqlite3_prepare(db, "SELECT COUNT(*) FROM rooms", -1, &stmt, NULL);
	sqlite3_step(stmt);
	n_rooms = sqlite3_column_int(stmt, 0);
	sqlite3_finalize(stmt);

	sqlite3_prepare(db, "SELECT COUNT(*) FROM batches", -1, &stmt, NULL);
	sqlite3_step(stmt);
	n_batches = sqlite3_column_int(stmt, 0);
	sqlite3_finalize(stmt);

	sqlite3_prepare(db, "SELECT COUNT(*) FROM allocations", -1, &stmt, NULL);
	sqlite3_step(stmt);
	n_allocs = sqlite3_column_int(stmt, 0);
	sqlite3_finalize(stmt);

	allocs = g_malloc(sizeof(Allocation) * n_allocs);

	sqlite3_prepare(db, "SELECT * FROM allocations", -1, &stmt, NULL);
	gint i = 0;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		allocs[i] = Allocation_from_stmt(stmt);
		i++;
	}
	sqlite3_finalize(stmt);

	return (Meta) {
		.n_teachers = n_teachers,
		.n_batches  = n_batches,
		.n_time_slots = n_time_slots,
		.n_rooms    = n_rooms,
		.n_allocs   = n_allocs,
		.allocs     = allocs,
		.mutate_swaps = mutate_swaps,
		.fitness_penalty_time_clash_batch = fitness_penalty_time_clash_batch,
		.fitness_penalty_time_clash_teacher = fitness_penalty_time_clash_teacher,
		.n_population = n_population
	};
}
