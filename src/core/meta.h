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
	gint n_teachers;
	gint n_batches;
	gint n_time_slots;
	gint n_time_slots_per_day;
	gint n_rooms;
	gint n_allocs;
	Allocation *allocs;
	/* const gint crossover_points; */ /* Fixed at 2 for now */
	gint   mutate_swaps;
	gfloat fitness_penalty_time_clash_teacher;
	gfloat fitness_penalty_time_clash_batch;
	gint   n_population;

} Meta;

Meta Meta_from_db(sqlite3 *db);

#endif
