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


#ifndef DB_TABLES_META_H
#define DB_TABLES_META_H 1

#include "../libs.h"

typedef struct Meta {
	gint n_teachers;
	gint n_batches;
	gint n_time_slots;
	gint n_time_slots_per_day;
	gint n_rooms;
	gint n_allocs;
	/* const gint crossover_points; */ /* Fixed at 2 for now */
	gint   mutate_swaps;
	gfloat fitness_penalty_time_clash_teacher;
	gfloat fitness_penalty_time_clash_batch;
	gfloat fitness_penalty_room_capacity;
	gint   n_population;
	gint   db_schedules_valid;
} Meta;

Meta Meta_from_db(sqlite3 *db);
void insert_Meta(sqlite3 *db, const Meta *meta);
void init_db_with_Meta(sqlite3 *db, const Meta *meta);


#endif
