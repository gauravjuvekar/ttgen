#ifndef DB_TABLES_META_H
#define DB_TABLES_META_H 1

#include <glib.h>
#include "../sqlite/sqlite3.h"

#include "allocations.h"

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
	gint   n_generations;
	gint   db_schedules_valid;
} Meta;

Meta Meta_from_db(sqlite3 *db);
void insert_Meta(sqlite3 *db, const Meta *meta);
void init_db_with_Meta(sqlite3 *db, const Meta *meta);


#endif
