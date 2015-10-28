#ifndef CORE_META_H
#define CORE_META_H 1

#include <glib.h>
#include "../sqlite/sqlite3.h"

typedef struct Allocation {
	gint teacher;
	gint batch;
	gint subject;
} Allocation;

Allocation Allocation_from_stmt(sqlite3_stmt *stmt);

typedef struct Meta {
	const gint n_teachers;
	const gint n_batches;
	const gint n_slots;
	const gint n_rooms;
	const gint n_allocs;
	const Allocation *const allocs;
	/* const gint crossover_points; */ /* Fixed at 2 for now */
	const gint mutate_swaps;
	const gfloat fitness_penalty_time_clash_teacher;
	const gfloat fitness_penalty_time_clash_batch;
	const gint n_population;

} Meta;

Meta Meta_from_db(sqlite3 *db);

#endif
