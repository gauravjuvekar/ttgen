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


#ifndef CORE_GENETIC_H
#define CORE_GENETIC_H 1

#include "../libs.h"

#include "../db_tables/meta.h"
#include "../db_tables/allocations.h"
#include "../db_tables/batches.h"
#include "../db_tables/rooms.h"


typedef struct Schedule {
	gint   *time_slots;
	gfloat fitness;

	gint *allocations;
} Schedule;

Schedule *Schedule_init(const Meta *meta);
void Schedule_seed_random(Schedule *schedule,
                          const Meta *meta,
                          const Allocation allocs[],
                          const Batch      batches[],
                          const Room       rooms[]);
Schedule *Schedule_clone(const Schedule *schedule, const Meta *meta);
void Schedule_free(Schedule *schedule);
void Schedule_mutate(Schedule *schedule,
                     const Meta *meta,
                     const Allocation allocs[],
                     const Batch      batches[],
                     const Room       rooms[]);
void Schedule_crossover(const Schedule *mother, const Schedule *father,
	    			    Schedule **daughter,    Schedule **son,
                        const Meta *meta,
                        const Allocation allocs[],
                        const Batch      batches[],
                        const Room       rooms[]);

gfloat Schedule_fitness(const Schedule *schedule,
                        const Meta *meta,
                        const Allocation allocs[],
                        const Batch      batches[],
                        const Room       rooms[]);
gint Schedule_compare(const Schedule *a, const Schedule *b);
gint Schedule_compare_wrapper(const Schedule **a, const Schedule **b);

void Schedule_print(const Schedule *schedule,
                    const Meta *meta,
                    const Allocation allocs[],
                    const Batch      batches[],
                    const Room       rooms[]);

static inline gint room_from_slot(gint slot, const Meta *meta) {
	return slot % meta->n_rooms;
}
static inline gint time_slot_from_slot(gint slot, const Meta *meta) {
	return slot / meta->n_rooms;
}

#endif
