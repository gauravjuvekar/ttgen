#include "../sqlite/sqlite3.h"
#include <glib.h>
#include "genetic.h"


static inline gint slot_from_array(const gint *array, gint slot, gint room,
                                   const Meta *meta) {
	return array[slot * meta->n_rooms + room];
}


static void assert_Schedule_valid(const Schedule *schedule, const Meta *meta) {
	gint alloc;
	for(alloc = 0; alloc < meta->n_allocs; alloc++) {
		g_assert(schedule->time_slots[schedule->allocations[alloc]] == alloc);
	}
}

Schedule *Schedule_init(const Meta *meta) {
	Schedule *schedule = g_new(Schedule, 1);
	*schedule = (Schedule) {
		.time_slots = g_malloc(sizeof(*(schedule->time_slots)) *
		                       meta->n_time_slots * meta->n_rooms),
		.fitness = 0,
		.allocations = g_malloc(sizeof(*(schedule->allocations)) *
		                        meta->n_allocs)
	};
	gint slot;
	for(slot = 0; slot < meta->n_time_slots * meta->n_rooms; slot++) {
		schedule->time_slots[slot] = -1;
	}
	return schedule;
}

void Schedule_print(const Schedule *schedule,
                    const Meta *meta,
                    const Allocation allocs[]) {
	gint room;
	for(room = 0; room < meta->n_rooms; room++) {
		gint time_slot;
		for(time_slot = 0; time_slot < meta->n_time_slots; time_slot++) {
			gint slot = slot_from_array(schedule->time_slots, time_slot, room,
			                            meta);
			if (slot == -1) {
				g_print("(          )\t");
			}
			else {
				Allocation alloc = allocs[slot];
				g_print("(T%d, B%d, S%d)\t",
				        alloc.teacher, alloc.batch, alloc.subject);
			}
		}
		g_print("\n");
	}
}


static gint Schedule_find_vacant(const Schedule *schedule, gint current, const Meta *meta) {
		gint right;
		gint left;
		left = right = current;
		g_assert(left >= 0);
		g_assert(right < meta->n_time_slots * meta->n_rooms);
		while (((schedule->time_slots[right] != -1) &&
		        (schedule->time_slots[left]  != -1)) &&
		       !((left == 0) &&
		         (right == (meta->n_time_slots * meta->n_rooms)))) {
			if (right + 1 < meta->n_time_slots * meta->n_rooms) {
				right++;
			} if (left - 1 >= 0) {
				left--;
			}
			g_assert(left >= 0);
			g_assert(right < meta->n_time_slots * meta->n_rooms);
		}
		if (schedule->time_slots[right] == -1) {
			return right;
		}
		if (schedule->time_slots[left]  == -1) {
			return left;
		}
		g_assert(FALSE); /* Atleast one vacant slot must be found */
}


void Schedule_seed_random(Schedule *schedule,
                          const Meta *meta,
                          const Allocation allocs[]) {
	/* The caller should check if a allocatable solution exists */
	g_assert(meta->n_allocs <= meta->n_rooms * meta->n_time_slots);
	gint alloc;
	for(alloc = 0; alloc < meta->n_allocs; alloc++) {
		gint slot = g_random_int_range(0, meta->n_time_slots * meta->n_rooms);
		gint empty = Schedule_find_vacant(schedule, slot, meta);
		schedule->time_slots[empty] = alloc;
		schedule->allocations[alloc] = empty;
	}
	assert_Schedule_valid(schedule, meta);
	schedule->fitness = Schedule_fitness(schedule, meta, allocs);
}


Schedule *Schedule_clone(const Schedule *schedule, const Meta *meta) {
	assert_Schedule_valid(schedule, meta);
	Schedule *clone = g_new(Schedule, 1);
	*clone = (Schedule) {
		.time_slots = g_memdup(schedule->time_slots,
		                       sizeof(*(schedule->time_slots)) *
		                       meta->n_rooms * meta->n_time_slots),
		.fitness = schedule->fitness,
		.allocations = g_memdup(schedule->allocations,
		                        sizeof(*(schedule->allocations)) *
		                        meta->n_allocs)
	};
	assert_Schedule_valid(clone, meta);
	return clone;
}


void Schedule_free(Schedule *schedule) {
	g_free(schedule->time_slots);
	g_free(schedule->allocations);
	g_free(schedule);
}


gfloat Schedule_fitness(const Schedule *schedule,
                        const Meta *meta,
                        const Allocation allocs[]) {
	assert_Schedule_valid(schedule, meta);

	gint alloc;
	gfloat fitness = 0;


	for(alloc = 0; alloc < meta->n_allocs; alloc++) {
		gint slot = schedule->allocations[alloc];
		gint time_slot = time_slot_from_slot(slot, meta);

		gint time_clash_teacher = -1;
		gint time_clash_batch = -1;
		gint room;
		for (room = 0; room < meta->n_rooms; room++) {
			const gint allocation = slot_from_array(schedule->time_slots,
			                                        time_slot, room, meta);
			if (allocation == -1) continue;
			if (allocs[allocation].teacher ==
			    allocs[alloc].teacher) {
				time_clash_teacher += 1;
			}
			if (allocs[allocation].batch ==
			    allocs[alloc].batch) {
				time_clash_batch += 1;
			}
		}
		g_assert(time_clash_batch > -1);
		g_assert(time_clash_teacher > -1);
		fitness += (time_clash_teacher *
		            meta->fitness_penalty_time_clash_teacher);
		fitness += (time_clash_batch *
		            meta->fitness_penalty_time_clash_batch);
	}
	return fitness;
}


gint Schedule_compare(const Schedule *a, const Schedule *b) {
	/* Fittest first */

	if (a->fitness > b->fitness) {
		if ((a->fitness - b->fitness) < (10 * FLT_EPSILON)) {
			return 0;
		}
		else {
			return -1;
		}
	}
	else if (a->fitness < b->fitness) {
		if ((b->fitness - a->fitness) < (10 * FLT_EPSILON)) {
			return 0;
		}
		else {
			return 1;
		}
	}
	else {
		return 0;
	}
}

gint Schedule_compare_wrapper(const Schedule **a, const Schedule **b) {
	return Schedule_compare(*a, *b);
}


void Schedule_mutate(Schedule *schedule,
                     const Meta *meta,
                     const Allocation allocs[]) {
	assert_Schedule_valid(schedule, meta);
	gint swaps;
	for(swaps = 0; swaps < meta->mutate_swaps; swaps++) {
		gint first  = g_random_int_range(0, meta->n_allocs);
		gint second = g_random_int_range(0, meta->n_allocs);

		gint tmp;
		tmp = schedule->allocations[first];
		schedule->allocations[first] = schedule->allocations[second];
		schedule->allocations[second] = tmp;

		schedule->time_slots[schedule->allocations[first]]  = first;
		schedule->time_slots[schedule->allocations[second]] = second;
	}
	assert_Schedule_valid(schedule, meta);
	schedule->fitness = Schedule_fitness(schedule, meta, allocs);
}


void Schedule_crossover(const Schedule *mother, const Schedule *father,
                        Schedule **daughter,    Schedule **son,
                        const Meta *meta, const Allocation allocs[]) {
	assert_Schedule_valid(mother, meta);
	assert_Schedule_valid(father, meta);

	gint crossover_1 = g_random_int_range(0, meta->n_allocs);
	gint crossover_2 = g_random_int_range(crossover_1, meta->n_allocs + 1);

	*daughter = Schedule_clone(mother, meta);
	*son      = Schedule_clone(father, meta);
	assert_Schedule_valid(*son, meta);
	assert_Schedule_valid(*daughter, meta);

	gint i;
	for(i = crossover_1; i < crossover_2; i++) {
		/* Free the time slots of children */
		(*son     )->time_slots[(*son     )->allocations[i]] = -1;
		(*daughter)->time_slots[(*daughter)->allocations[i]] = -1;
	}

	for(i = crossover_1; i < crossover_2; i++) {
		/* Swap son and daughter elements */
		gint tmp;
		tmp = (*son)->allocations[i];
		(*son)->allocations[i] = (*daughter)->allocations[i];
		(*daughter)->allocations[i] = tmp;

		(*son    )->allocations[i] =
			Schedule_find_vacant(*son,      (*son     )->allocations[i], meta);
		(*daughter)->allocations[i] =
			Schedule_find_vacant(*daughter, (*daughter)->allocations[i], meta);
		(*son     )->time_slots[(*son     )->allocations[i]] = i;
		(*daughter)->time_slots[(*daughter)->allocations[i]] = i;
	}
	assert_Schedule_valid(*son, meta);
	assert_Schedule_valid(*daughter, meta);
	(*son)->fitness      = Schedule_fitness(*son, meta, allocs);
	(*daughter)->fitness = Schedule_fitness(*daughter, meta, allocs);
}
